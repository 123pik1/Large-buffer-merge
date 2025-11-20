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

LargeBufferMerge::LargeBufferMerge(unsigned int bufferSizeHint, string inputFile, string outputFile)
    : bufferSize(bufferSizeHint == 0 ? 1 : bufferSizeHint), inputFile(inputFile), outputFile(outputFile)

{
    readCount = 0;
    writeCount = 0;
    largeBufferSize = bufferSize / SMALL_BUFFER_SIZE;
}

LargeBufferMerge::~LargeBufferMerge()
{
    cleanup();
}

void LargeBufferMerge::sort()
{
    // TODO
    startMenu();
    readCount = 0;
    writeCount = 0;
    mergeCounter=0;

    Tape inputTape(inputFile);
    // TODO
    inputTape.printTape();
    {
        Tape outputTape(outputFile);
        outputTape.clearTape();
    }

    createInitialRuns(inputTape);

    while (currentTape < allTapes - 1)
    {
        tapeEndForThisPhase = allTapes;

        while (currentTape < tapeEndForThisPhase - 1)
            mergeRuns(tapeEndForThisPhase);
        // TODO

        interMenu();
        mergeCounter++;
    }

    moveToOutput();
    printStats();
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

bool LargeBufferMerge::mergeRuns(unsigned int limit)
{
    unsigned int effectiveEnd = (limit > 0) ? limit : allTapes;
    int numInputs = min(static_cast<int>(largeBufferSize - 1), static_cast<int>(effectiveEnd - currentTape));
    if (numInputs < 2)
        return false;
    // w Tape'ach są przetrzymywane bloki pamięci wielkość SMALL_BUFFER_SIZE
    vector<Tape> inputTapes;
    for (int i = 0; i < numInputs; i++)
    {
        inputTapes.emplace_back(Tape(makeRunFilename(currentTape++)));
    }

    Tape outputTape(makeRunFilename(allTapes++));
    outputTape.clearTape();

    priority_queue<QueueEntry, vector<QueueEntry>, QueueEntryComparator> pq;

    vector<Number> writeBuffer;

    for (size_t i = 0; i < inputTapes.size(); ++i)
    {
        if (!inputTapes[i].isEmpty())
        {
            pq.push({inputTapes[i].getCurrNumber(), static_cast<unsigned int>(i)});
        }
    }

    // Zapisujemy liczby z pq do buforu wielkości strony dyskowej (mały bufor)
    while (!pq.empty())
    {
        QueueEntry entry = pq.top();
        pq.pop();

        writeBuffer.push_back(entry.value);

        if (writeBuffer.size() == bufferSize)
        {
            flushBuffer(outputTape, writeBuffer);
        }

        Tape &tape = inputTapes[entry.runIndex];
        tape.readNextNumber();
        if (!tape.isEmpty())
        {
            pq.push({tape.getCurrNumber(), entry.runIndex});
        }
    }

    if (!writeBuffer.empty())
    {
        flushBuffer(outputTape, writeBuffer);
    }

    outputTape.writePage();

    // allTapes +=numInputs;
    writeCount += static_cast<unsigned int>(outputTape.getWriteCounter());
    for (const auto &tape : inputTapes)
    {
        readCount += static_cast<unsigned int>(tape.getReadCounter());
    }
    return true;
}

// zapisuje bufor do taśmy
void LargeBufferMerge::flushBuffer(Tape &outputTape, vector<Number> &buffer)
{
    for (const auto &value : buffer)
        outputTape.appendNumber(value);
    buffer.clear();
}

// usuwa pliki tymczasowe
void LargeBufferMerge::cleanup() const
{
    namespace fs = std::filesystem;
    std::string dir = "tapes";

    try
    {
        fs::remove_all(dir);
        fs::create_directory(dir);
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error podczas czyszczenia folderu: " << e.what() << endl;
    }
}

// przenosi plik w miejsce gdzie powinien znajdować się plik z rezultatem
void LargeBufferMerge::moveToOutput()
{
    using namespace filesystem;
    path src;
    path dest;

    src = path(makeRunFilename(allTapes - 1));
    dest = path(outputFile);

    error_code ec;
    rename(src, dest, ec);
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
        break;
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
        inputFile = "data/exampleData";
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
    unsigned long long currentPrintTape = currentTape;
    for (; currentPrintTape < allTapes; currentPrintTape++)
    {
        Tape tape(makeRunFilename(currentPrintTape));
        tape.printTape();
    }
}

void LargeBufferMerge::printStats()
{
    cout << "Posortowane: \n";
    Tape outTape(outputFile);
    outTape.printTape();

    cout << "==== Statystyki ====\n";
    cout << "Liczba faz " << mergeCounter << endl;
    cout << "Liczba zapisow: " << writeCount << endl;
    cout << "Liczba odczytów: " << readCount << endl;
    cout << "suma dostępów do dysku: "<<readCount+writeCount<<endl;
}