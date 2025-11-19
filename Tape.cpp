#include "Tape.hpp"
#include <iostream>
using namespace std;

// konieczne dla algorytmu
Tape::Tape(string filename) : filename(filename), currNmb(""), beginningPos(0), empty(false)
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

    file.clear();
    file.seekg(beginningPos);

    int itemsRead = 0;
    for (int i = 0; i < SMALL_BUFFER_SIZE; i++)
    {
        if (file >> nmb)
        {
            empty = false;
            Number number(nmb);
            readPageTab[i] = number;
            itemsRead++;

            // update beginningPos immediately after each successful extraction
            streampos pos = file.tellg();
            if (pos != static_cast<streampos>(-1))
            {
                beginningPos = pos;
            }
            else
            {
                // if tellg is invalid, try to move to EOF and use that position
                file.clear();
                file.seekg(0, ios::end);
                pos = file.tellg();
                if (pos != static_cast<streampos>(-1))
                    beginningPos = pos;
                // leave stream state as-is; next loop iteration will see EOF
            }
        }
        else
        {
            if (i == 0)
                empty = true;
            break;
        }
    }

    if (itemsRead > 0)
        elementOnReadPage = 0;
    else
        elementOnReadPage = SMALL_BUFFER_SIZE;

    readCounter++;
}

void Tape::readNextNumber()
{
    if (elementOnReadPage >= SMALL_BUFFER_SIZE)
    {
        readPage();
    }
    if (elementOnReadPage >= SMALL_BUFFER_SIZE)
    {
        currNmb.setNumberString("");
        return;
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
    file.clear();
    beginningPos = 0;
    file.seekg(beginningPos);
    file.seekp(0, ios::end);
}

void Tape::writePage()
{
    if (elementOnWritePage == 0)
        return;
    file.clear();
    file.seekp(0, ios::end);
    for (int i = 0; i < elementOnWritePage; i++)
    {
        if (!writePageTab[i].isEmpty())
        {
            file << writePageTab[i].getNumberString() << '\n';
            empty = false;
        }
    }
    writeCounter++;
    file.flush();
    resetWritePage();
}

void Tape::appendNumber(const Number &nmb)
{
    if (!nmb.isEmpty())
        empty = false;
    if (elementOnWritePage >= SMALL_BUFFER_SIZE)
    {
        writePage();
    }
    writePageTab[elementOnWritePage] = nmb;
    elementOnWritePage++;
}

bool Tape::isEmpty()
{
    if (elementOnReadPage < SMALL_BUFFER_SIZE && !readPageTab[elementOnReadPage].isEmpty())
        return false;
    if (!currNmb.isEmpty())
        return false;
    file.clear();
    file.seekg(beginningPos);
    string dummy;
    bool hasData = static_cast<bool>(file >> dummy);
    file.clear();
    file.seekg(beginningPos);

    return !hasData;
}

void Tape::goToBegin()
{
    file.clear();
    file.seekg(beginningPos);
}

// debugowanie + wymogi zadania
void Tape::printTape()
{
    file.clear();
    file.seekg(beginningPos);
    cout << "wyswietlanie tasmy " << filename << '\n';
    if (!currNmb.isEmpty())
        cout << currNmb.getNumberString() << '\n';
    for (int i = elementOnReadPage; i < SMALL_BUFFER_SIZE; i++)
        if (!readPageTab[i].isEmpty())
            cout << readPageTab[i].getNumberString() << '\n';
    string nmb;
    while (file >> nmb)
        cout << nmb << '\n';
    file.clear();
    file.seekg(beginningPos);
}

void Tape::deletePrevRecords()
{
    file.clear();
    file.seekg(beginningPos);

    ofstream tempFile(tempTapeLocation);
    if (!tempFile.is_open())
    {
        cerr << "deletePrevRecords: failed to open temp file: " << tempTapeLocation << '\n';
        return;
    }

    string nmb;
    while (file >> nmb)
    {
        tempFile << nmb << '\n';
    }

    file.close();
    tempFile.close();

    if (remove(filename.c_str()) != 0)
    {
        cerr << "deletePrevRecords: failed to remove file: " << filename << '\n';
        // attempt to reopen original file
        initFile();
        return;
    }
    if (rename(tempTapeLocation, filename.c_str()) != 0)
    {
        cerr << "deletePrevRecords: failed to rename temp file to: " << filename << '\n';
        initFile();
        return;
    }

    initFile();
    resetTape();
}

void Tape::clearTape()
{
    runsOnTape = 0;
    empty = true;
    if (file.is_open())
        file.close();

    if (remove(filename.c_str()) != 0)
    {
        cerr << "clearTape: failed to remove file: " << filename << '\n';
    }
    beginningPos = 0;
    initFile();
}

// pomocnicze

void Tape::initFile()
{
    file.open(filename, ios::in | ios::out | ios::ate);
    if (!file.is_open())
    {
        ofstream createFile(filename);
        createFile.close();
        file.open(filename, ios::in | ios::out | ios::ate);
    }
    beginningPos = 0;
    file.seekg(beginningPos);
}

void Tape::resetWritePage()
{
    for (int i = 0; i < SMALL_BUFFER_SIZE; i++)
    {
        writePageTab[i].setNumberString("");
    }
    elementOnWritePage = 0;
}

void Tape::resetReadPage()
{
    for (int i = 0; i < SMALL_BUFFER_SIZE; i++)
    {
        readPageTab[i].setNumberString("");
    }
    elementOnReadPage = SMALL_BUFFER_SIZE;
}