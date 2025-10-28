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
    for (int i = 0; i < pageSize; i++)
    {
        if (file >> newNumber)
            currentReadPage[i].setNumberString(newNumber);
        else
            currentReadPage[i].setNumberString("");
    }
    elementOnReadPage = 0;
    if (file.eof() || file.fail())
        file.clear();
}

Number Tape::readNextNumber()
{
    if (elementOnReadPage < pageSize && currentReadPage[0].getNumberString() != "")
    {
        return getNextFromPage();
    }
    // resetReadPage();
    readPage();
    return getNextFromPage();
}

Number Tape::getNextFromPage()
{
    currentNumber = currentReadPage[elementOnReadPage];
    elementOnReadPage++;
    return currentNumber;
}

void Tape::resetReadPage()
{
    for (int i = 0; i < pageSize; i++)
    {
        currentReadPage[i].setNumberString("");
    }
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

// ...existing code...
bool Tape::isEmpty()
{
    // If we already have a current number -> not empty
    if (currentNumber.getNumberString() != "")
        return false;

    // If there are unread numbers in the current read page -> not empty
    for (int i = elementOnReadPage; i < pageSize; ++i)
        if (currentReadPage[i].getNumberString() != "")
            return false;

    // Probe the underlying file without consuming data:
    streampos pos = file.tellg();
    // Ensure stream is ready for read attempt
    file.clear();
    string tmp;
    bool hasMore = static_cast<bool>(file >> tmp);

    // restore stream state and position
    file.clear();
    if (pos != -1)
        file.seekg(pos);
    else
        file.seekg(0, ios::cur);

    return !hasMore;
}
// ...existing code...

void Tape::clearTape()
{
    file.close();
    remove(filename.c_str());
    ofstream createFile(filename);
    createFile.close();
    file.open(filename, std::ios::in | std::ios::out);
    currentBeginningPos = 0;
    resetReadPage();
    resetWritePage();
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
    file.clear();
    file.seekp(0, ios::end);

    if (elementOnWritePage < pageSize)
    {
        writeToPage(nmb);
        return;
    }
    writePage();
    writeToPage(nmb);
}

void Tape::writeToPage(Number nmb)
{
    currentWritePage[elementOnWritePage] = nmb;
    elementOnWritePage++;
}


void Tape::resetWritePage()
{
    for (int i=0; i<pageSize;i++)
    {
        currentWritePage[i].setNumberString("");
    }
    elementOnWritePage =0;
}


void Tape::writePage()
{
    for (int i=0;i<pageSize;i++)
    {
        file << currentWritePage[i].getNumberString()<<endl;
    }
    file.flush();
    resetWritePage();
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
    resetReadPage(); resetWritePage();
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