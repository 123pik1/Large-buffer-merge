#include "PolyphaseSort.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
PolyphaseSort::PolyphaseSort(int numTapes, const string &inputFile, const string &outputFile)
    : numTapes(numTapes), inputFile(inputFile), outputFile(outputFile)
{
    for (int i = 0; i < numTapes; ++i)
    {
        tapes.push_back(new Tape("tapes/tape" + to_string(i) + ".txt"));
    }
}

PolyphaseSort::~PolyphaseSort()
{
    for (Tape *tape : tapes)
    {
        string nmb = tape->getFilename();
        delete tape;
        remove(nmb.c_str());
    }
}

void PolyphaseSort::sort()
{
    startMenu();
    // 1. Dystrybucja początkowa
    distributeInitialRuns();

    for (auto tape : tapes)
    {
        tape->goToBegin();
    }

    // Number nmb1("10679022"), nmb2("-151920.8");
    // cout << (nmb1<nmb2)<<endl;
    // 2. Merge na jedną taśmę
    // while (!isSorted())
    mergePhase();

    for (Tape *tape : tapes)
    {
        readCounter += tape->getReadCounter();
        writeCounter += tape->getWriteCounter();
    }
    // 3. Skopiowanie na taśmę outputu
    copyToOutput();

    
}

void PolyphaseSort::distributeInitialRuns()
{
    Tape inputTape(inputFile);
    inputTape.printTape();
    int tapeIndex = 0;
    Number prev;
    bool first = true;

    int fibCurrent = 1;
    int fibNext = 1;
    int runCount = 0;

    while (!inputTape.isEmpty())
    {
        Number curr = inputTape.getCurrNumber();
        if (first || !(curr < prev))
        {
            tapes[tapeIndex]->appendNumber(curr);
            prev = curr;
            first = false;
        }
        else
        {

            runCount++;
            prev = curr;
            first = false;
            if (runCount >= fibCurrent)
            {

                tapeIndex = (tapeIndex + 1) % (numTapes - 1);
                runCount = 0;

                int temp = fibCurrent;
                fibCurrent = fibNext;
                fibNext = temp + fibNext;
            }
            tapes[tapeIndex]->appendNumber(curr);
        }
        inputTape.readNextNumber();
    }

    for (auto tape : tapes)
    {
        tape->writePage();
    }
    readCounter += inputTape.getReadCounter();
    writeCounter += inputTape.getWriteCounter();
}

/****
 * 1. znalezienie pustej taśmy
 * 2. uznanie jej za destynację
 * 3. wybranie taśm początkowych
 * 4. MergeOneRun
 * 5. Powtarzaj 4 póki nie powstanie nowa pusta
 */
void PolyphaseSort::mergePhase()
{
    int iterations = 0;
    // sortuje póki jest więcej niż jedna niepusta taśma
    while (countNonEmpty() > 1)
    {
        // Find empty tape
        int idEmpty = -1;
        for (int i = 0; i < tapeNumber; i++)
        {
            if (tapes[i]->isEmpty())
            {
                idEmpty = i;
                break;
            }
        }

        if (idEmpty == -1)
        {
            cerr << "ERROR: No empty tape found!" << endl;
            break;
        }


        while (countNonEmpty() >= 2)
        {
            mergeOneRun(idEmpty);
        }

        tapes[idEmpty]->writePage();

    }
}

bool PolyphaseSort::isSorted()
{
    int nonEmptyCount = 0;
    for (auto tape : tapes)
    {
        if (!tape->isEmpty())
        {
            nonEmptyCount++;
        }
    }
    return nonEmptyCount <= 1;
}

void PolyphaseSort::copyToOutput()
{
    Tape *sortedTape = nullptr;
    for (auto tape : tapes)
    {
        if (!tape->isEmpty())
        {
            sortedTape = tape;
            break;
        }
    }

    if (sortedTape)
    {
        Tape outputTape(outputFile);
        outputTape.clearTape();
        sortedTape->goToBegin();
        while (!sortedTape->isEmpty())
        {
            Number num = sortedTape->getCurrNumber();
            outputTape.appendNumber(num);
            sortedTape->readNextNumber();
        }
        outputTape.writePage();
        cout << "==== Stats ====" << endl;
        cout << "liczba merge'y " << mergeCounter << endl;
        cout << "liczba zapisów " << writeCounter << endl;
        cout << "liczba odczytów " << readCounter << endl;
        cout << "suma dostępów do dysku " << readCounter + writeCounter << endl;
        cout << "Tasma wynikowa: \n";
        outputTape.printTape();
    }
    
}

int PolyphaseSort::findEmpty()
{
    int destIndex = -1;
    for (int i = 0; i < numTapes; ++i)
    {
        if (tapes[i]->isEmpty())
        {
            destIndex = i;
            break;
        }
    }
    return destIndex;
}

/***
 * 1. tworzy kolejkę priorytetową - posortowaną wg wielkości sortowanych liczb
 * 2. wrzuca na nią liczby z każdej dostępnej taśmy wejściowej
 * 3. sprawdza czy kolejka jest pusta, jeżeli tak zakończ
 * 4. zapisuje nowy numer w destination
 * 5. odczytuje kolejny numer z taśmy
 * 6. porównuje z poprzednim odczytanym z danej taśmy
 * 7. jeżeli nowy numer jest >= zapisuje go do kolejki
 * 8. wróć do 3
 */
void PolyphaseSort::mergeOneRun(int idEmpty)
{
    interMenu();
    mergeCounter++;
    // czy run się skonczył na danej taśmie
    bool tapeHasData[tapeNumber];
    // ostatnia liczba zapisana z taśmy
    Number lastFromTape[tapeNumber];

    for (int i = 0; i < tapeNumber; i++)
    {
        tapeHasData[i] = !tapes[i]->isEmpty();
    }

    while (true)
    {
        // index najmniejszej liczby
        int idLowest = findMinimumAmongActive(idEmpty, tapeHasData);
        // brak najmniejszej liczby - wychodzi
        if (idLowest == -1)
            break;
        Number current = tapes[idLowest]->getCurrNumber();

        tapes[idEmpty]->appendNumber(current);
        lastFromTape[idLowest] = current; // zapisuje ostatni numer który został przekazany
        tapes[idLowest]->readNextNumber();

        // Check if run ended: next number is smaller than current
        if (tapes[idLowest]->isEmpty() ||
            lastFromTape[idLowest].isHigherThan(tapes[idLowest]->getCurrNumber()))
        {
            tapeHasData[idLowest] = false;
        }

        // Check if all runs finished
        bool anyTapeActive = false;
        for (int i = 0; i < tapeNumber; i++)
        {
            if (tapeHasData[i])
            {
                anyTapeActive = true;
                break;
            }
        }
        if (!anyTapeActive)
            break;
    }
}

int PolyphaseSort::findMinimumAmongActive(int idEmpty, bool *tapeHasData)
{
    // id najmniejszej
    int idLowest = -1;
    for (int i = 0; i < tapeNumber; i++)
    {

        if (!tapeHasData[i])
            // if (tapes[i]->isEmpty())
            continue;

        if (idLowest == -1 ||
            tapes[i]->getCurrNumber().isLowerThan(tapes[idLowest]->getCurrNumber()))
        {
            idLowest = i;
        }
    }
    return idLowest;
}

int PolyphaseSort::countNonEmpty()
{
    int counter = 0;
    for (Tape *tape : tapes)
    {
        if (!tape->isEmpty())
            counter++;
    }
    return counter;
}

void PolyphaseSort::writePages()
{
    for (Tape *tape : tapes)
    {
        tape->writePage();
    }
}

void PolyphaseSort::printTapes()
{
    for (Tape *tape : tapes)
        tape->printTape();
}

// wymogi zadania
void PolyphaseSort::startMenu()
{
    int choice;
    cout << "\n=== start menu ===" << endl;
    cout << "1. Wygeneruj dane" << endl;
    cout << "2. Ile merge'y przed kolejnym menu"<<endl;
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
        autoMerge=true;
    default:
        break;
    }
}

void PolyphaseSort::runPyScript()
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

void PolyphaseSort::enterData()
{
    Tape inputTape(inputFile);
    cout << "chcesz wyczyścic wejście? y/n\n";
    string answ;
    cin >> answ;
    if (answ=="y")
        inputTape.clearTape();
    cout << "ile liczb?\n";
    int n;
    cin >> n;
    for (int i=0; i<n;i++)
    {
        string nmb;
        cin >> nmb;
        inputTape.appendNumber(Number(nmb));
    }
}

void PolyphaseSort::enterEntryFile()
{
    cin >> inputFile;
}

void PolyphaseSort::interMenu()
{
    if (autoMerge)
        return;
    if (mergesToPrintMenu>0)
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
        cin>>mergesToPrintMenu;
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