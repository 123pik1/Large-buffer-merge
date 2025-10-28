#include "Tape.h"
#include <string>
#include <iostream>
#include "constants.h"
using namespace std;



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
    if (iterationsToPause > 0)
        iterationsToPause--;
    if (iterationsToPause>0 || isAuto)
        return;
    cout << "\
    1. Auto\n\
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
        return;
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