#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Tape.h"
#include "Fibbonaci.h"
using namespace std;

// liczba taśm wykorzystywanych do sortowania
// liczba taśm nie mniejsza od 3
#define tapeNumber 3
// lokalizacja pliku z którego jest wczytywanie
#define inputFile "data/exampleData"
// bazowa nazwa pliku z taśmą
#define baseFileName "tapes/tape"
// lokalizacja pliku do którego jest zapisany wynik
#define outputFile "output/result"

// zlicza dostępy do pamięci
int memoryAccessCounter = 0;
// zlicza fazy sortowania
int sortPhasesCounter = 0;

// TODO
/*TODO
- proper memory access counter
- sort phase counter
- menu użytkownika
*/

bool isAuto = true;

void printTab(int *tab, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << tab[i] << " ";
    }
}

int countEmpty(Tape **tapes)
{
    int emptyCount = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->printTape();
        if (tapes[i]->isEmpty())
            emptyCount++;
    }
    return emptyCount;
}

int findNonEmpty(Tape **tapes)
{
    for (int i = 0; i < tapeNumber; i++)
    {
        if (!tapes[i]->isEmpty())
        {
            cout << "niepusta znaleziona: " << i << endl;
            return i;
        }
    }
    return 0;
}

int countNonEmpty(Tape **tapes)
{
    return tapeNumber - countEmpty(tapes);
}

int findMinimumAmongActive(Tape **tapes, int idEmpty, bool *tapeHasData)
{
    int idLowest = -1;
    for (int i = 0; i < tapeNumber; i++)
    {
        if (!tapeHasData[i])
            continue;

        if (idLowest == -1 ||
            tapes[i]->getCurrentNumber().isLowerThan(tapes[idLowest]->getCurrentNumber()))
        {
            idLowest = i;
        }
    }
    return idLowest;
}

void mergeOneRun(Tape **tapes, int idEmpty)
{
    bool tapeHasData[tapeNumber];
    Number lastFromTape[tapeNumber]; // Track last number from each tape

    for (int i = 0; i < tapeNumber; i++)
    {
        tapeHasData[i] = (i != idEmpty && !tapes[i]->isEmpty());
    }

    while (true)
    {
        int idLowest = findMinimumAmongActive(tapes, idEmpty, tapeHasData);

        if (idLowest == -1)
            break;

        Number current = tapes[idLowest]->getCurrentNumber();
        tapes[idEmpty]->appendNumber(current);

        // Store current number before moving to next
        lastFromTape[idLowest] = current;
        tapes[idLowest]->readNextNumberAndDelete();

        // Check if run ended: next number is smaller than current
        if (tapes[idLowest]->isEmpty() ||
            lastFromTape[idLowest].isHigherThan(tapes[idLowest]->getCurrentNumber()))
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

void merging(Tape **tapes, int idEmpty)
{
    while (countNonEmpty(tapes) >= 2)
        mergeOneRun(tapes, idEmpty);
    // while (true)
    // {
    //     cout << "w drugim while true - merging" << endl;
    //     // znalezienie najmniejszej liczby
    //     int idLowest = -1;
    //     for (int i = 0; i < tapeNumber; i++)
    //     {
    //         if (i == idEmpty)
    //             continue;
    //         if (tapes[i]->isEmpty() || tapes[i]->getCurrentNumber().getNumberString() == "")
    //         {
    //             cout << "jedna taśma spuściała" << endl;
    //             return;
    //         }
    //         if (idLowest == -1)
    //             idLowest = i;
    //         cout<<tapes[idLowest]->getCurrentNumber().getNumberString()<<" comparing with: "<<tapes[i]->getCurrentNumber().getNumberString()<<endl;
    //         if (tapes[idLowest]->getCurrentNumber().isHigherThan(tapes[i]->getCurrentNumber()))
    //         {
    //             idLowest = i;
    //         }
    //     }
    //     cout << "dodaje liczbe do pustej taśmy: " << tapes[idLowest]->getCurrentNumber().getNumberString() << " z " << tapes[idLowest]->filename << endl;
    //     // dodanie liczby do pustej taśmy
    //     tapes[idEmpty]->appendNumber(tapes[idLowest]->getCurrentNumber());
    //     tapes[idLowest]->readNextNumberAndDelete();
    // }
}

// zwraca id niepustej taśmy
int sort(Tape **tapes)
{
    cout << "początek sortowania" << endl;
    // sortuje póki jest więcej niż jedna niepusta taśma
    while (countNonEmpty(tapes) > 1)
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
            cout << "ERROR: No empty tape found!" << endl;
            break;
        }

        // Reset all tapes to beginning
        for (int i = 0; i < tapeNumber; i++)
        {
            tapes[i]->resetToBeginning();
        }

        cout << "pusta taśma znaleziona: " << idEmpty << endl;

        // Merge phase
        merging(tapes, idEmpty);

        cout << "taśmy zmergowane" << endl;
        cout << "fazy sortowania: " << ++sortPhasesCounter << endl;
    }
    cout << "posortowane" << endl;
    // sprawdza która taśma jest niepusta
    int nonEmpty = findNonEmpty(tapes);
    return nonEmpty;
}

// works
int countSeries()
{
    Number actualNmb, previousNmb;
    Tape mainFile(inputFile);
    int series = 1;
    mainFile.readNextNumber();
    actualNmb = mainFile.getCurrentNumber();

    do
    {
        previousNmb = actualNmb;
        mainFile.readNextNumber();
        if (mainFile.getCurrentNumber().getNumberString() == "")
            break;
        actualNmb = mainFile.getCurrentNumber();
        // when previous is higher then adds series
        //? possible place to check
        if (actualNmb.isLowerThan(previousNmb))
            series++;
    } while (mainFile.getCurrentNumber().getNumberString() != "");

    return series;
}

void parseInputFile(Tape **tapes)
{
    Number previousNmb, currentNumber;
    int tapeID = 0;
    int totalSeries = countSeries();

    if (totalSeries == 0)
        return;

    Fibbonaci fibbo;
    int sum = 0;
    while (sum < totalSeries)
    {
        sum = 0;
        for (int i = 0; i < tapeNumber - 1; i++)
        {
            sum += fibbo.get_n_InFuture(i);
        }
        if (sum < totalSeries)
            fibbo.nextFibbo();
    }

    int distribution[tapeNumber];
    distribution[0] = fibbo.fibbo1;
    distribution[1] = fibbo.fibbo2;

    for (int i = 2; i < tapeNumber - 1; i++)
    {
        distribution[i] = fibbo.get_n_InFuture(i);
    }
    distribution[tapeNumber - 1] = 0;
    printTab(distribution, tapeNumber);
    Tape inputTape(inputFile);
    // inputTape.printTape(); //! wyświetlanie pliku przed sortowaniem
    cout << "fibbonaci zrobiony" << endl;
    cout << "pominelo ifa" << endl;
    int currentTapeId = 0;
    int seriesOnCurrentTape = 0;
    cout << "inicjalizacja intow i previousnmb" << endl;
    cout << "appenduje pierwsza liczbe" << endl;

    while (true)
    {
        inputTape.readNextNumber();
        cout << inputTape.file.tellg() << endl;
        cout << "Nowa liczba: " << inputTape.getCurrentNumber().getNumberString() << endl;
        if (inputTape.getCurrentNumber().getNumberString() == "")
            break;
        cout << "Ta liczba przeszla dalej: " << inputTape.getCurrentNumber().getNumberString() << endl;
        currentNumber = inputTape.getCurrentNumber();

        if (!currentNumber.isHigherThan(previousNmb))
        {
            seriesOnCurrentTape++;

            // when higher or equal because series are counted from 0
            if (seriesOnCurrentTape >= distribution[currentTapeId])
            {
                do
                {
                    currentTapeId = (currentTapeId + 1) % (tapeNumber - 1);
                } while (distribution[currentTapeId] == 0 && currentTapeId < tapeNumber - 1);

                seriesOnCurrentTape = 0;
            }
        }

        tapes[currentTapeId]->appendNumber(currentNumber);
        previousNmb = currentNumber;
    }
    cout << "wyjście z while true nr 1" << endl;
    for (int i = 0; i < tapeNumber; i++)
        tapes[i]->resetToBeginning();
    cout << "taśmy zresetowane" << endl;
}

void prepareTapes(Tape **tapes)
{
    parseInputFile(tapes);
}

void newMain()
{
    Tape *tapes[tapeNumber];
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i] = new Tape(string(baseFileName) + to_string(i));
        tapes[i]->clearTape();
    }
    prepareTapes(tapes);
    int id = sort(tapes);
    Tape outputTape(outputFile);
    tapes[id]->copyTapeTo(&outputTape);
    outputTape.printTape();
}

int main()
{
    newMain();
    // Number nmb1("5.102");
    // Number nmb2("68586");
    // cout<<nmb1.isHigherThan(nmb2);
    // Tape tape1(inputFile);
    // Tape tape2(outputFile);
    // tape1.copyTapeTo(&tape2);
    // Tape *tapes[tapeNumber];
    // for (int i = 0; i < tapeNumber; i++)
    // {
    //     tapes[i] = new Tape(baseFileName + to_string(i));
    //     tapes[i]->clearTape();
    // }
    // prepareTapes(tapes);
    // sortIteration(tapes);
    // sortIteration(tapes);
    // sortIteration(tapes);

    // cout<<countEmpty(tapes)<<endl;

    // int id = sort(tapes);
    // Tape tape(std::string(baseFileName) + "2");
    // cout<<tape.isEmpty()<<endl;
}