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
    // 1. Dystrybucja początkowa
    distributeInitialRuns();

    for (auto tape : tapes)
    {
        tape->goToBegin();
    }

    // Number nmb1("10679022"), nmb2("-151920.8");
    // cout << (nmb1<nmb2)<<endl;
    // 2. Merge na jedną taśmę
    while (!isSorted())
        mergePhase();
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
    cout << "początek sortowania" << endl;
    int iterations = 0;
    // sortuje póki jest więcej niż jedna niepusta taśma
    while (countNonEmpty() > 1)
    {
        cout << "iteracja sortowania " << (++iterations) << endl;
        cout << "Non-empty tapes: " << countNonEmpty() << endl;

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
            cout << "ERROR: No empty tape found!" << endl;
            break;
        }



        cout << "pusta taśma znaleziona: " << idEmpty << endl;

        // Merge phase
        int mergeCount = 0;
        while (countNonEmpty() >= 2)
        {
            mergeOneRun(idEmpty);
            cout << "merge count: " << ++mergeCount << endl;
            printTapes();
        }
        // for (Tape *tape : tapes)
            // tape->deletePrevRecords();

        cout << "After merge phase:" << endl;
        for (int i = 0; i < tapeNumber; i++)
        {
            cout << "Tape " << i << " empty: " << tapes[i]->isEmpty() << endl;
        }
        tapes[idEmpty]->writePage();
    }
    cout << "posortowane" << endl;


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

// void PolyphaseSort::copyToOutput()
// {
//     Tape outputTape(outputFile);
//     outputTape.clearTape();
//     for (auto tape : tapes)
//     {
//         if (!tape->isEmpty())
//         {
//             tape->goToBegin();
//             while (!tape->isEmpty())
//             {
//                 Number num = tape->getCurrNumber();
//                 outputTape.appendNumber(num);
//                 tape->readNextNumber();
//             }
//         }
//     }
//     outputTape.writePage();
// }

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
        int idLowest = findMinimumAmongActive( idEmpty, tapeHasData);
        // brak najmniejszej liczby - wychodzi
        if (idLowest == -1)
            break;
        cout << tapes[idLowest]->getCurrNumber().getNumberString() << endl;
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
    int counter =0;
    for (Tape* tape : tapes)
    {
        if (!tape->isEmpty())
            counter++;
    }
    return counter;
}

void PolyphaseSort::writePages()
{
    for (Tape* tape:tapes)
    {
        tape->writePage();
    }
}

void PolyphaseSort::printTapes()
{
    for (Tape* tape:tapes)
        tape->printTape();
}
// TODO there are 4 records lost