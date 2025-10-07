#include "Number.h"
#include <string>
#include <fstream>

class Tape
{
    Number currentNumber;
    std::string filename;
    std::fstream file;

    void initFile(const std::string filename);
public:
    Tape(const std::string filename);
    Tape(const std::string filename, bool generate);
    ~Tape();


    // reads and deletes char from file
    char readAndDelete();
    // return currentNumber
    Number getCurrentNumber();
    // reads next number, deletes it from file and saves it tu currentNumber var
    // if empty -> sets currentNumber to empty string
    void readNextNumber();
    // returns if tape is empty (currentNumber is null and file is now empty)
    bool isEmpty();

    void appendNumber(Number nmb);

    void printTape();

    void clearTape();
};