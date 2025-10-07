#include <string>
#include <iostream>
#include <fstream>
#include "Tape.h"
using namespace std;

// liczba taśm wykorzystywanych do sortowania
#define tapeNumber 3
// lokalizacja pliku z którego jest wczytywanie
#define inputFile "data/exampleData"
// bazowa nazwa pliku z taśmą
#define baseFileName "tapes/tape"

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
            if (i==idEmpty)
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
        if (actualNmb.isHigherThan(previousNmb))
            series++;
    }
    while (mainFile.getCurrentNumber().getNumberString() != "");

    return series;
}

void parseInputFile(Tape** tapes)
{
    Number *actualNmb, *previousNmb;
    int tapeID = 0;
    int nmbOfSeries = countSeries();

}

void prepareTapes(Tape **tapes)
{
    for (int i=0; i<tapeNumber;i++)
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
    sort(tapes);
}