#include "Parsing.h"
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


// 1. inicjuje inputTape
// 2. tworzy tablicę zliczającą runy na każdej taśmie
// 3. inicjuje Fibbonaciego
// 4. Zczytuje pierwszy numer z inputu
// 5. ustawia tapeID na początkowe (0) i wstawia tam pierwszą liczbę z inputu
// 6. dla każdej kolejnej liczby:
// 6.1 sprawdza czy obecny numer jest mniejszy od poprzedniego (nowy run)
// 6.1.1 Jeżeli tak: sprawdza czy obecna liczba runów przekroczyła max na daną taśmę
// 6.1.1.1 Jeżeli tak: przechodzi do kolejnej taśmy, zwiększa fibbonaci current
// 6.1.2 zwiększa liczbę runów na obecnej taśmie
// 6.2 czyta kolejną liczbę
// 7. dopisuje strony do końca i resetuje taśmy
// 8. wyświetla wszystkie taśmy
void parseInputFile(Tape **tapes)
{
    Tape inputTape(inputFile);
    // entryMenu(inputTape);
    inputTape.printTape();
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
}

void prepareTapes(Tape **tapes)
{
    parseInputFile(tapes);
}