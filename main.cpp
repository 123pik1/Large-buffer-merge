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

void merging(Tape **tapes, int idEmpty)
{
    while (true)
    {
        // znalezienie najmniejszej liczby
        int idLowest = 0;
        for (int i = 0; i < tapeNumber; i++)
        {
            if (i == idEmpty)
                continue;
            if (tapes[i]->isEmpty())
                return;
            if (i == idLowest) //? optional - możliwe do usunięcia
                continue;
            if (tapes[idLowest]->getCurrentNumber().isHigherThan(tapes[i]->getCurrentNumber()))
            {
                idLowest = i;
            }
        }

        // dodanie liczby do pustej taśmy
        tapes[idEmpty]->appendNumber(tapes[idLowest]->getCurrentNumber());
        tapes[idLowest]->readNextNumber();
    }
}

// returns how much there are empty tapes
int sortIteration(Tape **tapes)
{
    // znalezienie pustej taśmy
    int idEmpty = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        if (tapes[i]->isEmpty())
        {
            idEmpty = i;
            break;
        }
    }

    // etap scalania taśm
    merging(tapes, idEmpty);

    // zlicza puste taśmy
    int emptyCount = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        if (tapes[i]->isEmpty())
            emptyCount++;
    }
    return emptyCount;
}

// zwraca id niepustej taśmy
int sort(Tape **tapes)
{
    // sortuje póki jest więcej niż jedna niepusta taśma
    while (tapeNumber - sortIteration(tapes) > 1)
        ;

    // sprawdza która taśma jest niepusta
    for (int i = 0; i < tapeNumber; i++)
    {
        if (!tapes[i]->isEmpty())
            return i;
    }
}

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
        if (mainFile.getCurrentNumber().getNumberString() != "")
            break;
        actualNmb = mainFile.getCurrentNumber();
        // when previous is higher then adds series
        //? possible place to check
        if (!actualNmb.isHigherThan(previousNmb))
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
    // TODO
    int sum = 0;
    while (sum < totalSeries)
    {
        sum = 0;
        for (int i = 0; i < tapeNumber; i++)
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

    // TODO
    Tape inputTape(inputFile);
    inputTape.printTape(); //! wyświetlanie pliku przed sortowaniem
    inputTape.readNextNumber();
    if (inputTape.getCurrentNumber().getNumberString() == "")
        return;
    
    previousNmb = inputTape.getCurrentNumber();
    int currentTapeId = 0;
    int seriesOnCurrentTape = 0;

    tapes[currentTapeId]->appendNumber(previousNmb);

    while(true)
    {
        inputTape.readNextNumber();
        if (inputTape.getCurrentNumber().getNumberString()=="")
            break;
        currentNumber = inputTape.getCurrentNumber();

        if (!currentNumber.isHigherThan(previousNmb))
        {
            seriesOnCurrentTape++;

            // when higher or equal because series are counted from 0
            if (seriesOnCurrentTape>=distribution[currentTapeId])
            {
                do
                {
                   currentTapeId = (currentTapeId+1)%(tapeNumber-1);
                } while (distribution[currentTapeId] == 0 && currentTapeId<tapeNumber-1);

                seriesOnCurrentTape = 0;
            }
        }

        tapes[currentTapeId]->appendNumber(currentNumber);
        previousNmb=currentNumber;
    }

    for (int i=0; i<tapeNumber; i++)
        tapes[i]->resetToBeginning();

}

void prepareTapes(Tape **tapes)
{
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->clearTape();
    }
    parseInputFile(tapes);
}

int main()
{
    Tape *tapes[tapeNumber];
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i] = new Tape(baseFileName + to_string(i));
    }
    prepareTapes(tapes);
    int id =sort(tapes);
    Tape outputTape(outputFile);
    tapes[id]->copyTapeTo(&outputTape);
}