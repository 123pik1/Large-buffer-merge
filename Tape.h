#include "Number.h"
#include <string>
#include <fstream>

#define tempTapeLocation "tapes/tmp"

class Tape
{
    Number currentNumber;
    // filestream is open
    std::streampos currentBeginningPos;

    void initFile(const std::string filename);
    void deletePreviousRecords();
public:
    Tape(const std::string filename);
    ~Tape();
    std::fstream file;
    std::string filename;

    // return currentNumber
    Number getCurrentNumber();
    // reads next number and saves it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    void readNextNumber();
    // reads next number, deletes it from file and saves it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    void readNextNumberAndDelete();
    // returns if tape is empty (currentNumber is null and file is now empty)
    // !!!! be carefull functions also resets positon !!!!
    bool isEmpty();

    void appendNumber(Number nmb);

    void printTape();

    void clearTape();


    void resetToBeginning();

    void copyTapeTo(Tape *newTape);

};