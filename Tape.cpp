#include "Tape.hpp"
using namespace std;

// konieczne dla algorytmu
Tape::Tape(string filename) : filename(filename), currNmb(""),beginningPos(0)
{
    initFile();
    readPage();
}

Tape::~Tape()
{
    if (file.is_open())
    {
        file.close();
    }
}

void Tape::readPage()
{
    string nmb;
    file.seekg(beginningPos);
    for (int i=0; i<pageSize; i++)
    {
        if (file>>nmb)
        {
            empty=false;
            Number number(nmb);
            readPageTab[i]=number;
            beginningPos=file.tellg();
        }
        else if (i==0)
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
    if (currNmb.getNumberString()=="")
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
    for (int i=0; i<pageSize;i++)
    {
        if (writePageTab[i].getNumberString()!="")
        {
            empty=false;
            file << writePageTab[i].getNumberString() << endl;
        }
    }
    writeCounter++;
    elementOnWritePage=0;
}

void Tape::appendNumber(Number nmb)
{
    if (nmb.getNumberString()!="")
        empty = false;
    if (elementOnWritePage>=pageSize)
    {
        writePage();
    }
    writePageTab[elementOnWritePage]=nmb;
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
//TODO
void Tape::printTape()
{
    file.seekg(beginningPos);
    
}

void Tape::deletePrevRecords()
{
    
}


// pomocnicze

void Tape::initFile()
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