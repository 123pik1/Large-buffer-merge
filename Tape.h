#include "Number.h"
#include <string>
#include <fstream>

#define pageSize 4 // page size in bytes is 4*8=32
#define tempTapeLocation "tapes/tmp"

class Tape
{
    int readCounter = 0;
    int writeCounter = 0;
    Number currentNumber;
    // filestream is open
    std::streampos currentBeginningPos = 0;
    std::streampos currentReadPos = 0;
    Number currentReadPage[pageSize];
    int elementOnPage = 0;

    void initFile(const std::string filename);
    void readPage();

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
    bool isEmpty();

    void appendNumber(Number nmb);

    void printTape();

    void clearTape();

    void resetToBeginning();

    void copyTapeTo(Tape *newTape);

    int getWriteCounter();
    int getReadCounter();
};