#include "LargeBufferMerge.hpp"
#include <algorithm>
#include <cstdio>
#include <memory>
#include <queue>
#include <utility>

using namespace std;
namespace
{
    std::string makeRunFilename(std::size_t index)
    {
        return "tapes/run_" + std::to_string(index) + ".tmp";
    }

    struct QueueEntry
    {
        Number value;
        std::size_t runIndex;
    };

    struct QueueEntryComparator
    {
        bool operator()(const QueueEntry &lhs, const QueueEntry &rhs) const
        {
            return rhs.value < lhs.value;
        }
    };
}

LargeBufferMerge::LargeBufferMerge(std::size_t bufferSizeHint, string inputFile, string outputFile)
    : bufferSize(bufferSizeHint == 0 ? 1 : bufferSizeHint), inputFile(inputFile), outputFile(outputFile)

{
    readCount = 0;
    writeCount = 0;
}

void LargeBufferMerge::sort()
{
    readCount = 0;
    writeCount = 0;

    Tape inputTape(inputFile);
    Tape outputTape(outputFile);
    outputTape.clearTape();

    const auto runFiles = createInitialRuns(inputTape);

    if (runFiles.empty())
    {
        outputTape.writePage();
        writeCount += static_cast<std::size_t>(outputTape.getWriteCounter());
        cleanup(runFiles);
        return;
    }

    mergeRuns(runFiles, outputTape);
    cleanup(runFiles);
}

std::vector<std::string> LargeBufferMerge::createInitialRuns(Tape &inputTape)
{
    std::vector<std::string> runFiles;
    std::vector<Number> buffer;
    buffer.reserve(bufferSize);

    std::size_t runIndex = 0;
    while (!inputTape.isEmpty())
    {
        buffer.clear();
        for (std::size_t i = 0; i < bufferSize && !inputTape.isEmpty(); ++i)
        {
            buffer.push_back(inputTape.getCurrNumber());
            inputTape.readNextNumber();
        }

        if (buffer.empty())
            break;

        std::sort(buffer.begin(), buffer.end(), [](const Number &lhs, const Number &rhs)
                  { return lhs < rhs; });

        const std::string runPath = makeRunFilename(runIndex++);
        {
            Tape runTape(runPath);
            runTape.clearTape();

            for (const auto &num : buffer)
                runTape.appendNumber(num);

            runTape.writePage();
            writeCount += static_cast<std::size_t>(runTape.getWriteCounter());
        }

        runFiles.push_back(runPath);
    }

    readCount += static_cast<std::size_t>(inputTape.getReadCounter());
    return runFiles;
}

void LargeBufferMerge::mergeRuns(const std::vector<std::string> &runFiles, Tape &outputTape)
{
    std::vector<std::unique_ptr<Tape>> runTapes;
    runTapes.reserve(runFiles.size());

    for (const auto &path : runFiles)
    {
        auto tape = std::make_unique<Tape>(path);
        tape->goToBegin();
        runTapes.push_back(std::move(tape));
    }

    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueEntryComparator> heap;

    for (std::size_t idx = 0; idx < runTapes.size(); ++idx)
    {
        if (runTapes[idx]->isEmpty())
            continue;

        Number value = runTapes[idx]->getCurrNumber();
        heap.push(QueueEntry{value, idx});
        runTapes[idx]->readNextNumber();
    }

    std::vector<Number> buffer;
    buffer.reserve(bufferSize);

    while (!heap.empty())
    {
        QueueEntry smallest = heap.top();
        heap.pop();

        buffer.push_back(smallest.value);
        if (buffer.size() >= bufferSize)
            flushBuffer(outputTape, buffer);

        auto &sourceTape = runTapes[smallest.runIndex];
        if (!sourceTape->isEmpty())
        {
            Number nextValue = sourceTape->getCurrNumber();
            heap.push(QueueEntry{nextValue, smallest.runIndex});
            sourceTape->readNextNumber();
        }
    }

    if (!buffer.empty())
        flushBuffer(outputTape, buffer);

    outputTape.writePage();

    for (const auto &tape : runTapes)
        readCount += static_cast<std::size_t>(tape->getReadCounter());
    writeCount += static_cast<std::size_t>(outputTape.getWriteCounter());
}

void LargeBufferMerge::flushBuffer(Tape &outputTape, std::vector<Number> &buffer)
{
    for (const auto &value : buffer)
        outputTape.appendNumber(value);
    buffer.clear();
}

void LargeBufferMerge::cleanup(const std::vector<std::string> &runFiles) const
{
    for (const auto &path : runFiles)
        std::remove(path.c_str());
}