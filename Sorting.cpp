#include "Sorting.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Tape.h"
#include "constants.h"
using namespace std;

extern void printTapes(Tape **tapes);


int countEmpty(Tape **tapes)
{
    int emptyCount = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->resetToBeginning();
        if (tapes[i]->isEmpty())
        {
            tapes[i]->printTape();
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
            cout << "pusta znaleziona: " << i << endl;
            tapes[i] -> printTape();
            return i;
        }
    }
    return -1;
}

int countNonEmpty(Tape **tapes)
{
    return tapeNumber - countEmpty(tapes);
}

int findMinimumAmongActive(Tape **tapes, int idEmpty, bool* isActive)
{
    // id najmniejszej
    int idLowest = -1;
    for (int i = 0; i < tapeNumber; i++)
    {

        if (i==idEmpty || !isActive[i])
            continue;

        if (idLowest == -1 ||
            tapes[i]->getCurrentNumber().isLowerThan(tapes[idLowest]->getCurrentNumber()))
        {
            idLowest = i;
        }
    }
    return idLowest;
}

// 1. Inicjuje tablicę zawierającą informację czy dana taśma ma aktywny run
// 2. porównanie dwóch liczb
// 3. zapisanie mniejszej do pustej taśmy
// 4. odczyt kolejnej liczby
// 5. powrót do pierwszego póki: poprzedni nie jest większy od obecnej liczby
void mergeOneRun(Tape **tapes, int idEmpty)
{
    // punkt 1
    bool activeTapes[tapeNumber];
    for (int i=0; i<tapeNumber;i++)
        activeTapes[i] = (i!=idEmpty && !tapes[i]->isEmpty());

    while (true)
    {
        // punkt 2
        int idOfMin = findMinimumAmongActive(tapes,idEmpty, activeTapes);
        // wykona się gdy wszystkie bedą puste
        if (idOfMin==-1)
        {
            cout << "All active runs ended. Exiting mergeOneRun." << endl;
            break;
        }
        Number minNumber = tapes[idOfMin]->getCurrentNumber();
        cout << "Prev: "<< minNumber.getNumberString()<<endl;
        // punkt 3
        tapes[idEmpty]->appendNumber(minNumber);
        // punkt 4
        Number prev = minNumber;
        tapes[idOfMin]->readNextNumberAndDelete();
        cout << "Curr: " << tapes[idOfMin]->getCurrentNumber().getNumberString() << endl;
        // punkt 5
        if (tapes[idOfMin]->isEmpty() || tapes[idOfMin]->getCurrentNumber().isLowerThan(prev))
        {
            activeTapes[idOfMin] = false;
            cout << "Deactivating tape " << idOfMin << endl;
        }
    }
    tapes[idEmpty]->writePage();
}



// 1. zlicza niepuste
// 2. jeżeli niepustych > 1 wykonuje akcje
// // 3. znajduje pustą
// 4. przeprowadza merge dla konkretnej pustej
// 5. Powtarza merge póki inna nie spuścieje
void merging(Tape **tapes, int idEmpty)
{
    cout <<"in merging"<<endl;
    int mergeCount = 0;
    while (tapes[idEmpty]->isEmpty() || findEmpty(tapes)==-1)
    {
        cout << "in merge while beginning "<<endl;
        


        // Merge phase
        mergeOneRun(tapes, idEmpty);
        cout << "pierwszy run się zrobił "<<endl;
        printTapes(tapes);
        //!
        // TODO blocking on the same run
        //!
        // while (findEmpty(tapes)==-1 && !tapes[idEmpty]->isEmpty())
        // {
        //     mergeOneRun(tapes, idEmpty);
        // }
        // it should not cross that point if 



        // interMediateMenu(tapes);
    }
    cout << "merge count: " << ++mergeCount << endl;
    cout<<"Empty tapes: "<<countEmpty(tapes)<<endl;
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