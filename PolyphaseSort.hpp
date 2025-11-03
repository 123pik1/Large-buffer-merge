#ifndef POLYPHASE_SORT_HPP
#define POLYPHASE_SORT_HPP

#include "Tape.hpp"
#include "constants.h"
#include <vector>

class PolyphaseSort
{
private:
    std::vector<Tape *> tapes;
    int outputTapeIndex;

    // Count runs on a tape
    int countRuns(Tape *tape);

    // Distribute initial runs from input to tapes
    void distributeRuns(Tape *inputTape);

    // Merge runs from input tapes to output tape
    int mergePhase();

    // Find tape with minimum current number
    int findMinTape(std::vector<int> &activeTapes);

public:
    PolyphaseSort();
    ~PolyphaseSort();

    void sort(const std::string &inputFile, const std::string &outputFile);
};

#endif