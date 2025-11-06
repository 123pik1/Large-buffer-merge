#include "LargeBufferMerge.hpp"
#include <algorithm>
#include <cstdio>
#include <memory>
#include <queue>
#include <utility>
#include <iostream>
using namespace std;

string makeRunFilename(size_t index)
{
    return "tapes/run_" + to_string(index) + ".tmp";
}

struct QueueEntry
{
    Number value;
    size_t runIndex;
};

struct QueueEntryComparator
{
    bool operator()(const QueueEntry &lhs, const QueueEntry &rhs) const
    {
        return rhs.value < lhs.value;
    }
};

LargeBufferMerge::LargeBufferMerge(size_t bufferSizeHint, string inputFile, string outputFile)
    : bufferSize(bufferSizeHint == 0 ? 1 : bufferSizeHint), inputFile(inputFile), outputFile(outputFile)

{
    readCount = 0;
    writeCount = 0;
    for (int i = 0; i < numTapes; ++i)
    {
        runTapes.push_back(new Tape("tapes/run_"+to_string(i)+".tmp"));
        runTapes.back()->clearTape();
    }
}

LargeBufferMerge::~LargeBufferMerge()
{
    for (Tape* tape : runTapes)
        delete tape;
}

void LargeBufferMerge::sort()
{
    // startMenu();
    //TODO
    readCount = 0;
    writeCount = 0;

    Tape inputTape(inputFile);
    Tape outputTape(outputFile);
    outputTape.clearTape();

    createInitialRuns(inputTape);
    mergeRuns(outputTape);

    // if (runFiles.empty())
    // {
    //     outputTape.writePage();
    //     writeCount += static_cast<size_t>(outputTape.getWriteCounter());
    //     cleanup(runFiles);
    //     return;
    // }

    // mergeRuns(runFiles, outputTape);
    // cleanup(runFiles);
}

void LargeBufferMerge::createInitialRuns(Tape &inputTape)
{
    std::vector<Number> buffer;
    buffer.reserve(bufferSize);

    int tapeIndex = 0;
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

        std::sort(buffer.begin(), buffer.end());
        for (const auto &num : buffer)
        {
            runTapes[tapeIndex]->appendNumber(num);
        }
        runTapes[tapeIndex]->writePage();
        tapeIndex = (tapeIndex + 1) % numTapes; // Cycle through tapes
    }

    readCount += static_cast<std::size_t>(inputTape.getReadCounter());
    for (auto &tape : runTapes)
    {
        writeCount += static_cast<std::size_t>(tape->getWriteCounter());
    }
}

void LargeBufferMerge::mergeRuns(Tape &outputTape)
{
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueEntryComparator> heap;
    for (int idx = 0; idx < numTapes; ++idx)
    {
        runTapes[idx]->goToBegin();
        if (!runTapes[idx]->isEmpty())
        {
            Number value = runTapes[idx]->getCurrNumber();
            heap.push(QueueEntry{value, static_cast<std::size_t>(idx)});
            runTapes[idx]->readNextNumber();
        }
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

    for (auto &tape : runTapes)
        readCount += static_cast<std::size_t>(tape->getReadCounter());
    writeCount += static_cast<std::size_t>(outputTape.getWriteCounter());
}

void LargeBufferMerge::flushBuffer(Tape &outputTape, vector<Number> &buffer)
{
    for (const auto &value : buffer)
        outputTape.appendNumber(value);
    buffer.clear();
}

void LargeBufferMerge::cleanup(const vector<string> &runFiles) const
{
    for (const auto &path : runFiles)
        remove(path.c_str());
}
//

//

//

//
//

//

//

////

//

//

//
// wymogi zadania
void LargeBufferMerge::startMenu()
{
    int choice;
    cout << "\n=== start menu ===" << endl;
    cout << "1. Wygeneruj dane" << endl;
    cout << "2. Ile merge'y przed kolejnym menu" << endl;
    cout << "3. Wprowadz dane z klawiatury" << endl;
    cout << "4. Załaduj z pliku" << endl;
    cout << "5. Ustaw tryb auto" << endl;
    cout << "Twój wybór: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        runPyScript();
        break;
    case 2:
        cin >> mergesToPrintMenu;
        break;
    case 3:
        enterData();
        break;
    case 4:
        enterEntryFile();
    case 5:
        autoMerge = true;
    default:
        break;
    }
}

void LargeBufferMerge::runPyScript()
{
    string command = "cd data; python3 generateData.py";
    int result = system(command.c_str());
    if (result != 0)
    {
        cout << "Problem ze skryptem: " << result << endl;
    }
    else
    {
        cout << "Skrypt wykonał sie szczęśliwie" << endl;
    }
}

void LargeBufferMerge::enterData()
{
    Tape inputTape(inputFile);
    cout << "chcesz wyczyścic wejście? y/n\n";
    string answ;
    cin >> answ;
    if (answ == "y")
        inputTape.clearTape();
    cout << "ile liczb?\n";
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string nmb;
        cin >> nmb;
        inputTape.appendNumber(Number(nmb));
    }
}

void LargeBufferMerge::enterEntryFile()
{
    cin >> inputFile;
}

void LargeBufferMerge::interMenu()
{
    if (autoMerge)
        return;
    if (mergesToPrintMenu > 0)
    {
        mergesToPrintMenu--;
        return;
    }
    int choice;
    cout << "\n=== inter menu ===" << endl;
    cout << "1. Ile merge'y przed kolejnym menu" << endl;
    cout << "2. Ustaw tryb auto" << endl;
    cout << "3. wyswietl tasmy" << endl;
    cout << "Twój wybór: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        cin >> mergesToPrintMenu;
        break;
    case 2:
        autoMerge = true;
        break;
    case 3:
        printTapes();
        break;
    default:
        break;
    }
}

void LargeBufferMerge::printTapes()
{
    
}