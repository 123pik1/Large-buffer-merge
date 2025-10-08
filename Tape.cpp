#include "Tape.h"
#include <random>
#include <iostream>
using namespace std;
Tape::Tape(const std::string filename) : filename(filename), currentNumber("")
{
    initFile(filename);
}

Tape::~Tape()
{
    if (file.is_open())
    {
        file.close();
        // remove(filename.c_str());
    }
}

void Tape::initFile(string filename)
{
    // Opens stream to file, if there is no file, creates it
    file.open(filename, ios::in | ios::out | ios::app);
    if (!file.is_open())
    {
        ofstream createFile(filename);
        createFile.close();
        file.open(filename, ios::in | ios::out | ios::app);
    }
}

void Tape::readNextNumber()
{
    string newNumber;
    if (!(file >> newNumber))
    {
        currentNumber.setNumberString("");
        return;
    }
    currentNumber.setNumberString(newNumber);
}

void Tape::readNextNumberAndDelete()
{
    readNextNumber();
    // deleting read part of file
    deletePreviousRecords();
}

//? maybe change to not deleting phisically but only logically
// by additional variable
void Tape::deletePreviousRecords()
{
    string rest;
    ofstream temp(tempTapeLocation);
    while (file >> rest)
        temp << rest << "\n";
    file.close();
    temp.close();

    remove(filename.c_str());
    rename(tempTapeLocation, filename.c_str());

    file.open(filename, std::ios::in | std::ios::out);
}

bool Tape::isEmpty()
{
    file.seekg(0, ios::end); // Pointer to end
    streampos size = file.tellg();
    file.seekg(currentBeginningPos); // Pointer to beginning
    return size - currentBeginningPos == 0 && currentNumber.getNumberString() == "";
}

void Tape::clearTape()
{
    file.close();
    remove(filename.c_str());
    ofstream createFile(filename);
    createFile.close();
    file.open(filename, std::ios::in | std::ios::out);
    currentBeginningPos = 0;
}

Number Tape::getCurrentNumber()
{
    return currentNumber;
}

void Tape::appendNumber(Number nmb)
{
    file << nmb.getNumberString() << "\n";
}

void Tape::printTape()
{
    resetToBeginning();
    string nmb;
    while (!(file>>nmb))
        cout << nmb <<"\n";
    resetToBeginning();
}

void Tape::resetToBeginning()
{
    file.seekg(currentBeginningPos);
    file.seekp(0, ios::end);
}

void Tape::copyTapeTo(Tape *newTape)
{
    newTape->clearTape();
    resetToBeginning();
    string nmb;
    while (file >> nmb)
    {
        newTape->appendNumber(nmb);
    }
    resetToBeginning();
}