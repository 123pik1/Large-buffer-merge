#include "Tape.h"
#include <random>
#include <iostream>
using namespace std;
Tape::Tape(const std::string filename) : filename(filename), currentNumber(""), currentBeginningPos(0)
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
    file.open(filename, ios::in | ios::out);
    if (!file.is_open())
    {
        ofstream createFile(filename);
        createFile.close();
        file.open(filename, ios::in | ios::out);
    }
}

void Tape::readNextNumber()
{
    string newNumber;
    // cout << "Pozycja przed wczytaniem: " << file.tellg() << endl;
    if (!(file >> newNumber))
    {
        currentNumber.setNumberString("");
        return;
    }
    // cout << "wczytana liczba: " << newNumber << "liczba znakow" << newNumber.length() << endl;
    // cout << file.tellg() << endl;
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
    streampos originalPos = file.tellg();
    file.seekg(0);

    string temp;
    bool empty = !(file >> temp);

    file.clear();
    file.seekg(originalPos);

    return empty;
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
    if (currentNumber.getNumberString() != "")
        return currentNumber;
    readNextNumber();
    return currentNumber;
}

void Tape::appendNumber(Number nmb)
{
    cout << "appenduje liczbe: " << nmb.getNumberString() << endl;
    resetToBeginning();

    file << nmb.getNumberString() << "\n";

    file.flush();
}

void Tape::printTape()
{
    streampos temp = file.tellg();
    resetToBeginning();
    string nmb;
    while ((file >> nmb))
        cout << nmb << "\n";
    resetToBeginning();
    file.seekg(temp);
}

void Tape::resetToBeginning()
{
    file.clear();
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