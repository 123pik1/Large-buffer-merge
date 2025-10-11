#include "Number.h"
#include <string>
#include <fstream>

#define tempTapeLocation "tapes/tmp"

class Tape
{
    Number currentNumber;
    // filestream is open
    std::streampos currentBeginningPos=0;
    std::streampos currentReadPos=0;

    void initFile(const std::string filename);
public:
    Tape(const std::string filename);
    ~Tape();
    std::fstream file;
    std::string filename;
    void deletePreviousRecords();

    // return currentNumber
    Number getCurrentNumber();
    // reads next number but does not save it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    Number readNextNumber();
    // reads next number, deletes it from file and saves it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    Number readNextNumberAndDelete();
    // returns if tape is empty (currentNumber is null and file is now empty)
    // !!!! be carefull functions also resets positon !!!!
    bool isEmpty();

    void appendNumber(Number nmb);

    void printTape();

    void clearTape();


    void resetToBeginning();

    void copyTapeTo(Tape *newTape);

};