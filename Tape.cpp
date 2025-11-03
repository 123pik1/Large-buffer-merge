#include "Tape.hpp"
#include <iostream>
using namespace std;

// konieczne dla algorytmu
Tape::Tape(string filename) : filename(filename), currNmb(""), beginningPos(0)
{
    initFile();
}

Tape::~Tape()
{
    if (elementOnWritePage > 0)
        writePage();
    if (file.is_open())
    {
        file.close();
    }
}

void Tape::readPage()
{
    resetReadPage();
    string nmb;
    file.seekg(beginningPos);
    for (int i = 0; i < pageSize; i++)
    {
        if (file >> nmb)
        {
            empty = false;
            Number number(nmb);
            readPageTab[i] = number;
            beginningPos = file.tellg();
        }
        else if (i == 0)
        {
            empty = true;
            break;
        }
    }
    elementOnReadPage = 0;
    readCounter++;
}

void Tape::readNextNumber()
{
    if (elementOnReadPage >= pageSize)
    {
        readPage();
    }
    currNmb = readPageTab[elementOnReadPage];
    elementOnReadPage++;
}

Number Tape::getCurrNumber()
{
    if (currNmb.getNumberString() == "")
        readNextNumber();
    return currNmb;
}

void Tape::resetTape()
{
    beginningPos = 0;
    file.seekg(beginningPos);
    file.seekp(0, ios::end);
}

void Tape::writePage()
{
    file.clear();
    file.seekp(0, ios::end);
    for (int i = 0; i < pageSize; i++)
    {
        if (!writePageTab[i].isEmpty())
        {
            file << writePageTab[i].getNumberString() << endl;
            empty = false;
        }
    }
    writeCounter++;
    elementOnWritePage = 0;
    file.flush();
    resetWritePage();
}

void Tape::appendNumber(Number nmb)
{
    if (nmb.getNumberString() != "")
        empty = false;
    if (elementOnWritePage >= pageSize)
    {
        writePage();
    }
    writePageTab[elementOnWritePage] = nmb;
    elementOnWritePage++;
}

bool Tape::isEmpty()
{
    return empty;
}

void Tape::goToBegin()
{
    file.seekg(beginningPos);
}

// debugowanie + wymogi zadania
void Tape::printTape()
{
    file.seekg(beginningPos);
    cout << "wyswietlanie tasmy " << filename << endl;
    file.clear();
    if (!currNmb.isEmpty())
        cout << currNmb.getNumberString() << endl;
    for (int i = elementOnReadPage; i < pageSize; i++)
        cout << readPageTab[i].getNumberString() << endl;
    string nmb;
    while (file >> nmb)
        cout << nmb << endl;
    file.clear();
    file.seekg(beginningPos);
}

void Tape::deletePrevRecords()
{
    ofstream tempFile(tempTapeLocation);
    string nmb;
    file.seekg(beginningPos);
    while (file>>nmb)
    {
        tempFile<<nmb<<endl;
    }
    //TODO
    file.close();
    tempFile.close();
    remove(filename.c_str());
    rename(tempTapeLocation, filename.c_str());
    resetTape();
}

void Tape::clearTape()
{
    file.close();
    remove(filename.c_str());
    initFile();
}

// pomocnicze

void Tape::initFile()
{
    file.open(filename, ios::in | ios::out);
    if (!file.is_open())
    {
        ofstream createFile(filename);
        createFile.close();
        file.open(filename, ios::in | ios::out);
    }
}

void Tape::resetWritePage()
{
    for (int i = 0; i < pageSize; i++)
    {
        writePageTab[i].setNumberString("");
    }
    elementOnWritePage = 0;
}

void Tape::resetReadPage()
{
    for (int i = 0; i < pageSize; i++)
    {
        readPageTab[i].setNumberString("");
    }
    elementOnReadPage = pageSize;
}