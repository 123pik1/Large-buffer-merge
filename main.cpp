#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Tape.h"
#include "Fibbonaci.h"
#include "constants.h"
using namespace std;

// zlicza dostępy do pamięci
int memoryAccessCounter = 0;
// zlicza fazy sortowania
int sortPhasesCounter = 0;
// zlicza zapisy
int saveCounter = 0;
// zlicza odczyty
int readCounter = 0;

// TODO
/*TODO
- proper memory access counter
- sort phase counter
- menu użytkownika
- określenie rozmiaru strony dyskowej - u mnie jeden rekord? - czy dodatkowa implementacja potrzebna
- zrobienie odczytywania stronami dyskowymi - w klasie Tape
*/
// TODO - change parsing input file to not count series

bool isAuto = false;
int iterationsToPause = 0;

void printTapes(Tape **tapes)
{
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->printTape();
    }
}

void runPythonScript()
{
    int result = system("python3 data/generateData.py");
    if (result != 0)
        cout << "problem with script" << endl;
}

void addNumber(Tape &mainTape)
{
    string number;
    cin >> number;
    mainTape.appendNumber(Number(number));
}

void interMediateMenu(Tape **tapes)
{
    cout << "1. Auto\n\
    2. Go n iteration in future\n\
    3. Print tapes\n\
    everything else: Continue"
         << endl;
    int option;
    cin >> option;
    switch (option)
    {
    case 1:
        isAuto = true;
        break;
    case 2:
        cin >> iterationsToPause;
        break;
    case 3:
        printTapes(tapes);
        break;
    default:
        break;
    }
}

void entryMenu(Tape &mainTape)
{
    cout << "\
    1. Auto\n\
    2. Append number\n\
    3. Generate Numbers\n\
    4. Go n iterations in future\n\
    everything else: Continue"
         << endl;
    int option;
    cin >> option;
    switch (option)
    {
    case 1:
        isAuto = true;
        break;
    case 2:
        addNumber(mainTape);
        break;
    case 3:
        runPythonScript();
        break;
    case 4:
        cin >> iterationsToPause;
        break;
    default:
        break;
    }
}

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

int findEmpty(Tape **tapes)
{
    for (int i = 0; i < tapeNumber; i++)
    {
        if (tapes[i]->isEmpty())
        {
            cout << "niepusta znaleziona: " << i << endl;
            return i;
        }
    }
    return -1;
}

int countNonEmpty(Tape **tapes)
{
    return tapeNumber - countEmpty(tapes);
}

int findMinimumAmongActive(Tape **tapes, int idEmpty, bool *tapeHasData)
{
    // id najmniejszej
    int idLowest = -1;
    for (int i = 0; i < tapeNumber; i++)
    {

        if (!tapeHasData[i])
            // if (tapes[i]->isEmpty())
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
        int idLowest = findMinimumAmongActive(tapes, idEmpty, tapeHasData);
        // brak najmniejszej liczby - wychodzi
        if (idLowest == -1)
            break;
        cout << tapes[idLowest]->getCurrentNumber().getNumberString() << endl;
        Number current = tapes[idLowest]->getCurrentNumber();

        tapes[idEmpty]->appendNumber(current);
        lastFromTape[idLowest] = current; // zapisuje ostatni numer który został przekazany
        tapes[idLowest]->readNextNumberAndDelete();

        // sprawdzenie czy kolejna liczba jest mniejsza od obecnej
        if (tapes[idLowest]->isEmpty() ||
            lastFromTape[idLowest].isHigherThan(tapes[idLowest]->getCurrentNumber()))
        {
            tapeHasData[idLowest] = false;
        }

        // sprawdzenie czy wszystkie runy są skończone
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
    int mergeCount = 0;
    while (countNonEmpty(tapes) >= 2)
    {
        mergeOneRun(tapes, idEmpty);
        cout << "merge count: " << ++mergeCount << endl;
        if (iterationsToPause > 0)
            iterationsToPause--;
        if (!isAuto && iterationsToPause == 0)
            interMediateMenu(tapes);
    }
}

// zwraca id niepustej taśmy
int sort(Tape **tapes)
{
    cout << "początek sortowania" << endl;
    int iterations = 0;
    // sortuje póki jest więcej niż jedna niepusta taśma
    while (countNonEmpty(tapes) > 1)
    {
        cout << "iteracja sortowania " << (++iterations) << endl;
        cout << "Non-empty tapes: " << countNonEmpty(tapes) << endl;

        //
        int idEmpty = findEmpty(tapes);

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

        cout << "After merge phase:" << endl;
        for (int i = 0; i < tapeNumber; i++)
        {
            cout << "Tape " << i << " empty: " << tapes[i]->isEmpty() << endl;
        }
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
        if (actualNmb.isLowerThan(previousNmb))
            series++;
    } while (mainFile.getCurrentNumber().getNumberString() != "");

    return series;
}

void parseInputFile(Tape **tapes)
{
    Tape inputTape(inputFile);
    entryMenu(inputTape);
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
    // printTab(distribution, tapeNumber);
    // inputTape.printTape(); //! wyświetlanie pliku przed sortowaniem
    // cout << "fibbonaci zrobiony" << endl;
    // cout << "pominelo ifa" << endl;
    int currentTapeId = 0;
    int seriesOnCurrentTape = 0;
    // cout << "inicjalizacja intow i previousnmb" << endl;
    // cout << "appenduje pierwsza liczbe" << endl;

    while (true)
    {
        inputTape.readNextNumber();
        // cout << inputTape.file.tellg() << endl;
        // cout << "Nowa liczba: " << inputTape.getCurrentNumber().getNumberString() << endl;
        if (inputTape.getCurrentNumber().getNumberString() == "")
            break;
        // cout << "Ta liczba przeszla dalej: " << inputTape.getCurrentNumber().getNumberString() << endl;
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
    // cout << "wyjście z while true nr 1" << endl;
    for (int i = 0; i < tapeNumber; i++)
        tapes[i]->resetToBeginning();
    // cout << "taśmy zresetowane" << endl;
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
    // outputTape.printTape();
}

int main()
{
    newMain();
}