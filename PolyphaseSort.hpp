#ifndef POLYPHASE_SORT_HPP
#define POLYPHASE_SORT_HPP

#include "Tape.hpp"
#include <vector>
#include <string>

class PolyphaseSort
{
private:
    std::vector<Tape *> tapes;
    int numTapes;
    std::string inputFile;
    std::string outputFile;

    // funkcje pomocnicze
    void distributeInitialRuns();
    void mergePhase();
    bool isSorted();
    void copyToOutput();

    int findEmpty();
    void mergeOneRun(int idEmpty);
    int findMinimumAmongActive(int idEmpty, bool *tapeHasData);
    int countNonEmpty();
    void writePages();
    void printTapes();

public:
    PolyphaseSort(int numTapes, const std::string &inputFile, const std::string &outputFile);
    ~PolyphaseSort();
    void sort();
};

#endif