#include "LargeBufferMerge.hpp"
#include <algorithm>
#include <cstdio>
#include <memory>
#include <queue>
#include <utility>
#include <iostream>
#include <filesystem>

using namespace std;

string makeRunFilename(unsigned int index)
{
    return "tapes/run_" + to_string(index) + ".tmp";
}

struct QueueEntry
{
    Number value;
    unsigned int runIndex;
};

struct QueueEntryComparator
{
    bool operator()(const QueueEntry &lhs, const QueueEntry &rhs) const
    {
        return rhs.value < lhs.value;
    }
};

LargeBufferMerge::LargeBufferMerge(unsigned int bufferSizeHint, string inputFile, string outputFile, unsigned int activeTapes)
    : bufferSize(bufferSizeHint == 0 ? 1 : bufferSizeHint), inputFile(inputFile), outputFile(outputFile), activeTapes(activeTapes)

{
    readCount = 0;
    writeCount = 0;
}

LargeBufferMerge::~LargeBufferMerge()
{
    cleanup();
}

void LargeBufferMerge::sort()
{
    //TODO
//    startMenu();
    readCount = 0;
    writeCount = 0;

    Tape inputTape(inputFile);
    {
        Tape outputTape(outputFile);
        outputTape.clearTape();
    }

    createInitialRuns(inputTape);

    while (currentTape < allTapes-1)
    {
        mergeRuns();
        mergeCounter++;
    }
    moveToOutput();
    
}

void LargeBufferMerge::createInitialRuns(Tape &inputTape)
{
    vector<string> runFiles;
    vector<Number> buffer;
    buffer.reserve(bufferSize);

    while (!inputTape.isEmpty())
    {
        // zapisanie do buforu BUFFER_SIZE rekordów
        buffer.clear();
        for (unsigned int i = 0; i < bufferSize && !inputTape.isEmpty(); ++i)
        {
            buffer.push_back(inputTape.getCurrNumber());
            inputTape.readNextNumber();
        }

        if (buffer.empty())
            break;
        // sortuje używając customowego porównania z klasy numer
        // std jest konieczne ze względu na namespace
        std::sort(buffer.begin(), buffer.end(), [](const Number &lhs, const Number &rhs)
                  { return lhs < rhs; });

        // zapisanie buforu do pliku
        const string runPath = makeRunFilename(allTapes++);
        {
            Tape runTape(runPath);
            runTape.clearTape();

            for (const Number &num : buffer)
                runTape.appendNumber(num);

            runTape.writePage();
            writeCount += static_cast<unsigned int>(runTape.getWriteCounter());
        }
    }
    readCount += static_cast<unsigned int>(inputTape.getReadCounter());
}

void LargeBufferMerge::mergeRuns()
{
    vector<Tape*> runTapes;
    for (int i = 0; i < activeTapes; i++)
    {
        Tape* tape = new Tape(makeRunFilename(currentTape++));
        tape->goToBegin();
        runTapes.push_back(tape);
    }
    Tape outputTape(makeRunFilename(allTapes++));

    priority_queue<QueueEntry, vector<QueueEntry>, QueueEntryComparator> heap;

    for (unsigned int idx = 0; idx < runTapes.size(); ++idx)
    {
        if (runTapes[idx]->isEmpty())
            continue;

        Number value = runTapes[idx]->getCurrNumber();
        heap.push(QueueEntry{value, idx});
        runTapes[idx]->readNextNumber();
    }

    vector<Number> buffer;
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

    for (const Tape* tape : runTapes)
    {
        readCount += static_cast<unsigned int>(tape->getReadCounter());
        delete tape;
    }
    writeCount += static_cast<unsigned int>(outputTape.getWriteCounter());
}

void LargeBufferMerge::flushBuffer(Tape &outputTape, vector<Number> &buffer)
{
    for (const auto &value : buffer)
        outputTape.appendNumber(value);
    buffer.clear();
}

void LargeBufferMerge::cleanup() const
{
    for (int i=0; i<allTapes;i++)
    {
        remove(makeRunFilename(i).c_str());
    }
}


void LargeBufferMerge::moveToOutput()
{
    using namespace filesystem;
    path src;
    path dest;

    src = path(makeRunFilename(allTapes-1));
    dest = path(outputFile);

    error_code ec;
    rename(src,dest,ec);
    if (!ec)
        return;

    if (!copy_file(src, dest, copy_options::overwrite_existing, ec) || ec)
    {
        std::cerr << "Problem ze skopiowaniem: " << ec.message() << '\n';
        return;
    }
    remove(src, ec);
    if (ec)
        std::cerr << "Problem z usunięciem pliku wejsciowego " << ec.message() << '\n';
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