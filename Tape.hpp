#pragma once

#include "constants.h"
#include "Number.hpp"
#include <string>
#include <fstream>

class Tape
{
private:
    // wymogi zadania
    int readCounter = 0;
    int writeCounter = 0;

    // konieczność dla algorytmu

    std::fstream file;
    std::string filename;

    Number currNmb;

    int elementOnWritePage = 0;

    int elementOnReadPage = SMALL_BUFFER_SIZE;

    bool empty;

    std::streampos beginningPos = 0;

    // metody:

public:
    // konieczność dla algorytmu

    Tape(std::string filename);
    ~Tape();

    Number writePageTab[SMALL_BUFFER_SIZE];
    Number readPageTab[SMALL_BUFFER_SIZE];

    void readNextNumber();
    Number getCurrNumber();

    void resetTape();
    void writePage();
    void appendNumber(const Number &nmb);
    bool isEmpty();
    void goToBegin();
    void deletePrevRecords();
    void clearTape();
    void readPage();

    // debugowanie + wymogi zadania
    void printTape();
    std::string getFilename() const
    {
        return filename;
    }
    int getReadCounter() const
    {
        return readCounter;
    }
    int getWriteCounter() const
    {
        return writeCounter;
    }

    int runsOnTape = 0;

    Tape(Tape &&other) noexcept = default;
    Tape &operator=(Tape &&other) noexcept = default;
    Tape(const Tape &) = delete;
    Tape &operator=(const Tape &) = delete;

private:
    // metody pomocnicze
    void initFile();
    void resetReadPage();
    void resetWritePage();
};