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


void parseInputFile(Tape **tapes)
{
    Tape inputTape(inputFile);
    // entryMenu(inputTape);
    cout << "Taśma wejściowa przeczytana" << endl;
    // printTab(distribution, tapeNumber);
    // inputTape.printTape(); //! wyświetlanie pliku przed sortowaniem
    // cout << "fibbonaci zrobiony" << endl;
    // cout << "pominelo ifa" << endl;
    int targetTapes = (tapeNumber > 1) ? (tapeNumber - 1) : 1;
    int currentTapeId = 0;

    bool firstNumber = true;
    Number previousNmb, currentNumber;

    // Stream input: read next number and append to current tape; when a number
    // is lower than previous -> new run -> advance tape (round-robin).
    inputTape.readNextNumber();
    while (inputTape.getCurrentNumber().getNumberString() != "")
    {
        currentNumber = inputTape.getCurrentNumber();

        if (firstNumber)
        {
            tapes[currentTapeId]->appendNumber(currentNumber);
            previousNmb = currentNumber;
            firstNumber = false;
        }
        else
        {
            if (currentNumber.isLowerThan(previousNmb))
            {
                if (targetTapes > 1)
                    currentTapeId = (currentTapeId + 1) % targetTapes;
                // if only one target tape, keep writing there
            }
            tapes[currentTapeId]->appendNumber(currentNumber);
            previousNmb = currentNumber;
        }

        inputTape.readNextNumber();
        // cout << "taśmy zresetowane" << endl;
    }
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->writePage();
        tapes[i]->resetToBeginning();
    }
}

void prepareTapes(Tape **tapes)
{
    parseInputFile(tapes);
}