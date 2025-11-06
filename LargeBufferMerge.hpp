#pragma once

#include "constants.h"
#include "Tape.hpp"
#include <cstddef>
#include <string>
#include <vector>

class LargeBufferMerge
{
public:
    explicit LargeBufferMerge(unsigned int bufferSize = LARGE_BUFFER_SIZE, std::string inputFile = InputFile, std::string outputFile = OutputFile, unsigned int activeTapes=tapeNumber);
    ~LargeBufferMerge();
    void sort();

    unsigned int getReadCount() const noexcept { return readCount; }
    unsigned int getWriteCount() const noexcept { return writeCount; }

private:
    unsigned int bufferSize;
    unsigned int readCount;
    unsigned int writeCount;
    unsigned int activeTapes;

    std::string inputFile;
    std::string outputFile;

    void createInitialRuns(Tape &inputTape);
    void mergeRuns();
    void flushBuffer(Tape &outputTape, std::vector<Number> &buffer);
    void cleanup() const;
    void moveToOutput();

    unsigned long long allTapes = 0;
    unsigned long long currentTape = 0;

    // wymogi zadania
    void startMenu();
    void interMenu(std::vector<Tape *> tapes);
    int mergeCounter = 0;
    bool autoMerge = false;
    int mergesToPrintMenu = 0;
    void runPyScript();
    void enterData();
    void enterEntryFile();
    void printTapes(std::vector<Tape *> tapes);
    void printStats();
};