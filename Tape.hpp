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

    Number writePageTab[pageSize];
    int elementOnWritePage = 0;

    Number readPageTab[pageSize];
    int elementOnReadPage = pageSize;

    bool empty;

    std::streampos beginningPos = 0;

    // metody:
    void readPage();

public:
    // konieczność dla algorytmu

    // reads next number from tape
    Tape(std::string filename);
    ~Tape();

    void readNextNumber();
    Number getCurrNumber();

    void resetTape();
    void writePage();
    void appendNumber(Number nmb);
    bool isEmpty();
    void goToBegin();
    void deletePrevRecords();


    // debugowanie + wymogi zadania
    void printTape();

private:
    // metody pomocnicze
    void initFile();
};