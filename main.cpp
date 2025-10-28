#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Tape.h"
#include "constants.h"
#include "Parsing.h"
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

extern void interMediateMenu(Tape **tapes);
extern void entryMenu(Tape &mainTape);
extern void printTapes(Tape **tapes);
extern void printTab(int *tab, int size);
extern void printTab(bool *tab, int size);

void newMain()
{
    Tape *tapes[tapeNumber];
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i] = new Tape(string(baseFileName) + to_string(i));
        tapes[i]->clearTape();
    }
    prepareTapes(tapes);
    // mergeOneRun(tapes,2);
    // int id = sort(tapes);
    // Tape outputTape(outputFile);
    // tapes[id]->copyTapeTo(&outputTape);
    // cout<<"id pustej "<<id<<endl;
    // outputTape.printTape();
    // printTapes(tapes);
}

int main()
{
    newMain();
}