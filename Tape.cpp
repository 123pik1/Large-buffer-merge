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

// readPage liczy się jako jeden dostęp odczytu
void Tape::readPage()
{
    readCounter++;
    string newNumber = "";
    int iterator = 0;
    for (int i = 0; i < pageSize; i++)
    {
        if (file >> newNumber)
            currentReadPage->setNumberString(newNumber);
        else
            currentReadPage->setNumberString("");
    }
}

Number Tape::readNextNumber()
{

    if (elementOnPage < pageSize)
    {
        currentNumber = currentReadPage[elementOnPage];
        elementOnPage++;
        return currentNumber;
    }
    elementOnPage = 0;

    string newNumber;
    file.seekg(currentReadPos);
    // cout << "Pozycja przed wczytaniem: " << file.tellg() << endl;
    if (!(file >> newNumber))
    {
        currentNumber.setNumberString("");
        return currentNumber;
    }
    // cout << "wczytana liczba: " << newNumber << "liczba znakow" << newNumber.length() << endl;
    // cout << file.tellg() << endl;
    currentReadPos = file.tellg();
    currentBeginningPos = file.tellg();
    currentNumber.setNumberString(newNumber);

    return currentNumber;
}

Number Tape::readNextNumberAndDelete()
{
    currentNumber = readNextNumber();
    deletePreviousRecords();
    return currentNumber;
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
    file.clear();
    currentReadPos = 0;
    currentBeginningPos = 0;
}

bool Tape::isEmpty()
{
    streampos originalPos = file.tellg();
    file.seekg(0);

    string temp;
    // cout << "sprawdza empty"<< temp << endl;
    bool empty = !(file >> temp);
    // if(empty)
    //     cout<<"it is empty "<< temp <<endl;
    file.clear();
    file.seekg(originalPos);

    return empty && currentNumber.getNumberString() == "";
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
    // cout << "appenduje liczbe: " << nmb.getNumberString() << endl;
    streampos currentRead = currentReadPos;
    file.seekp(0, ios::end);
    file << nmb.getNumberString() << "\n";
    file.flush();
    currentReadPos = currentRead;
}

void Tape::printTape()
{
    cout << "Wyświetlenie taśmy " << filename << endl;
    streampos temp = file.tellg();
    if (currentNumber.getNumberString() != "")
        cout << currentNumber.getNumberString() << endl;
    file.seekg(currentBeginningPos);
    string nmb;
    while ((file >> nmb))
        cout << nmb << "\n";
    file.seekg(temp);
    file.clear();
}

void Tape::resetToBeginning()
{
    file.clear();
    file.seekg(currentBeginningPos);
    currentReadPos = currentBeginningPos;
    file.seekp(0, ios::end);
}

void Tape::copyTapeTo(Tape *newTape)
{
    streampos originalPos = file.tellg();
    newTape->clearTape();
    Number nmb;
    if (currentNumber.getNumberString() != "")
        newTape->appendNumber(currentNumber);
    file.seekg(currentBeginningPos);
    string nmbStr;
    while (file >> nmbStr)
    {
        nmb.setNumberString(nmbStr);
        newTape->appendNumber(nmb);
    }
    file.clear();
    file.seekg(originalPos);
}