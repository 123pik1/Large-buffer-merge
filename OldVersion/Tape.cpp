#include "Tape.h"
#include <random>
#include <iostream>
#include <vector>
using namespace std;

Tape::Tape(const std::string filename) : filename(filename), currentNumber(""), currentBeginningPos(0)
{
    initFile(filename);
    resetReadPage();
    resetWritePage();
}

Tape::~Tape()
{
    if (file.is_open())
    {
        file.close();
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
    resetReadPage();
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
    elementOnReadPage = pageSize;
}

Number Tape::readNextNumberAndDelete()
{
    if (elementOnReadPage < pageSize)
    {
        Number nextNumber = getNextFromPage();

        if (elementOnReadPage == pageSize)
        {
            deletePreviousRecords();
            readPage();
        }

        return nextNumber;
    }
    // strona już pusta - czyta nową
    resetReadPage();
    deletePreviousRecords();
    readPage();
    return getNextFromPage();
}

// usuwa rekordy przez skopiowanie pozostałych do innego pliku
// następnie zamienia pliki miejscami
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
    if (currentNumber.getNumberString() != "")
        return false;
    for (int i = 0; i < pageSize; i++)
    {
        if (currentWritePage[i].getNumberString() != "")
            return false;
    }
    for (int i = 0; i < pageSize; ++i)
        if (currentReadPage[i].getNumberString() != "")
            return false;
    streampos pos = file.tellg();
    file.seekg(currentBeginningPos);
    file.clear(); // clear EOF/fail bits so we can try reading
    std::string tmp;
    bool hasMore = static_cast<bool>(file >> tmp);

    // restore stream state/position
    file.clear();
    if (pos != static_cast<streampos>(-1))
        file.seekg(pos);
    else
        file.seekg(0, ios::beg);

    return !hasMore;
}

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
    for (int i = 0; i < pageSize; i++)
    {
        currentWritePage[i].setNumberString("");
    }
    elementOnWritePage = 0;
}

void Tape::writePage()
{
    writeCounter++;
    for (int i = 0; i < pageSize; i++)
    {
        if (currentWritePage[i].getNumberString() != "")
            file << currentWritePage[i].getNumberString() << endl;
    }
    file.flush();
    resetWritePage();
}

void Tape::printTape()
{
    cout << "Wyświetlenie taśmy " << filename << endl;
    file.clear();
    streampos temp = file.tellg();
    if (currentNumber.getNumberString() != "")
        cout << currentNumber.getNumberString() << endl;
    file.seekg(currentBeginningPos);
    string nmb;
    while ((file >> nmb))
        cout << nmb << "\n";
    file.clear();
    file.seekg(temp);
    file.clear();
}

void Tape::resetToBeginning()
{
    file.clear();
    file.seekg(0, ios::beg);
    file.seekp(0, ios::beg);

    currentBeginningPos = 0;
    resetReadPage();
    resetWritePage();
    currentNumber.setNumberString("");
    currentReadPos = 0;
}

// copies only file content, ignores write and read pages - be aware
void Tape::copyTapeTo(Tape *newTape)
{
    resetToBeginning();
    newTape->clearTape();

    newTape->file.clear();
    newTape->file.seekp(0, ios::beg);

    string line;
    while (getline(file, line))
    {
        newTape->file << line << "\n";
    }
    newTape->file.flush();
    resetToBeginning();
    newTape->resetToBeginning();
}