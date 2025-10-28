#include "Parsing.h"
#include "Fibbonaci.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Tape.h"
#include "constants.h"
using namespace std;

extern void printTapes(Tape **tapes);
extern void printTab(int *tab, int size);

void parseInputFile(Tape **tapes)
{
    Tape inputTape(inputFile);
    // entryMenu(inputTape);
    inputTape.printTape(); //! wyświetlanie pliku przed sortowaniem
    int targetTapes = tapeNumber - 1;
    int currentTapeId = 0;
    int runsOnTapes[tapeNumber];
    for (int i = 0; i < tapeNumber; i++)
    {
        runsOnTapes[i] = 0;
    }

    unsigned long long fibPrev = 1;
    unsigned long long fibCurr = 1;

    bool firstNumber = true;
    Number previousNmb, currentNumber;
    // Używając Round-robin przechodzenie między dotępnymi taśmami i dodawanie rekordów do każdej z nich
    // używając rozkładu fibbonaciego
    inputTape.readNextNumber();
    while (inputTape.getCurrentNumber().getNumberString() != "")
    {
        currentNumber = inputTape.getCurrentNumber();
        cout << "current number " << currentNumber.getNumberString() << endl;
        if (firstNumber)
        {
            tapes[currentTapeId]->appendNumber(currentNumber);
            runsOnTapes[currentTapeId] = 1;
            previousNmb = currentNumber;
            firstNumber = false;
        }
        else
        {
            if (currentNumber.isLowerThan(previousNmb))
            {

                if (runsOnTapes[currentTapeId] >= fibCurr)
                {
                    currentTapeId = (currentTapeId + 1) % targetTapes;
                    unsigned long long fibNext = fibCurr + fibPrev;
                    fibPrev = fibCurr;
                    fibCurr = fibNext;
                }
                runsOnTapes[currentTapeId]++;
            }
            tapes[currentTapeId]->appendNumber(currentNumber);
            previousNmb = currentNumber;
        }

        inputTape.readNextNumber();
    }
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->writePage();
        tapes[i]->resetToBeginning();
    }

    printTapes(tapes);
    cout << "tablica z seriami: " << endl;
    printTab(runsOnTapes, tapeNumber);
}

void prepareTapes(Tape **tapes)
{
    parseInputFile(tapes);
}