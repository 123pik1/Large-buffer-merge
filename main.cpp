#include <string>
#include <iostream>
#include "Tape.h"
using namespace std;

// liczba taśm wykorzystywanych do sortowania
#define tapeNumber 4
// lokalizacja pliku z którego jest wczytywanie
#define inputFile "data/exampleData"

int memoryAccessCounter = 0;

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


void parseInputFile(Tape** tapes)
{

}


int main()
{
    string baseFileName = "tapes/tape";
    Tape *tapes[tapeNumber];
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i] = new Tape(baseFileName + to_string(i));
    }
    sort(tapes);
}