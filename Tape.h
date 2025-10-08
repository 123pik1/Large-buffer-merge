#include "Number.h"
#include <string>
#include <fstream>

#define tempTapeLocation "tapes/tmp.txt"

class Tape
{
    Number currentNumber;
    std::string filename;
    // filestream is open
    std::fstream file;
    std::streampos currentBeginningPos = 0;

    void initFile(const std::string filename);
    void deletePreviousRecords();
public:
    Tape(const std::string filename);
    ~Tape();

    // return currentNumber
    Number getCurrentNumber();
    // reads next number and saves it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    void readNextNumber();
    // reads next number, deletes it from file and saves it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    void readNextNumberAndDelete();
    // returns if tape is empty (currentNumber is null and file is now empty)
    bool isEmpty();

    void appendNumber(Number nmb);

    void printTape();

    void clearTape();


    void resetToBeginning();

    void copyTapeTo(Tape *newTape);
};