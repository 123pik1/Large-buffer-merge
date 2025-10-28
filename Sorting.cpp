#include "Sorting.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Tape.h"
#include "constants.h"
using namespace std;

int countEmpty(Tape **tapes)
{
    int emptyCount = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        if (tapes[i]->isEmpty())
        {
            emptyCount++;
        }
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
}

void merging(Tape **tapes, int idEmpty)
{
    int mergeCount = 0;
    while (countNonEmpty(tapes) >= 2)
    {
        mergeOneRun(tapes, idEmpty);
        cout << "merge count: " << ++mergeCount << endl;

        // interMediateMenu(tapes);
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
    cout << nonEmpty << endl;
    return nonEmpty;
}