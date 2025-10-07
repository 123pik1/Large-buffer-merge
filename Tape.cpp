#include "Tape.h"
#include <random>
using namespace std;
Tape::Tape(const std::string filename) : filename(filename), currentNumber("")
{
    initFile(filename);
}

Tape::Tape(const std::string filename, bool generate) : filename(filename), currentNumber("")
{
    initFile(filename);
    if (generate)
        generateTape(50,4,100);
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

Tape::~Tape()
{
    if (file.is_open())
    {
        file.close();
    }
}

bool Tape::isEmpty()
{
    file.seekg(0, ios::end); // Pointer to end
    streampos size = file.tellg();
    file.seekg(0); // Pointer to beginning
    return size == 0;
}

void Tape::generateTape(int count, int minDigits, int maxDigits)
{
    for (int i=0; i<count; i++)
    {

    }
}

Number Tape::getCurrentNumber()
{
    return currentNumber;
}