#pragma once

#include "constants.h"
#include "Tape.hpp"
#include <cstddef>
#include <string>
#include <vector>

class LargeBufferMerge
{
public:
    explicit LargeBufferMerge(unsigned int bufferSize = LARGE_BUFFER_SIZE, std::string inputFile = InputFile, std::string outputFile = OutputFile);

    void sort();

    unsigned int getReadCount() const noexcept { return readCount; }
    unsigned int getWriteCount() const noexcept { return writeCount; }

private:
    unsigned int bufferSize;
    unsigned int readCount;
    unsigned int writeCount;

    std::string inputFile;
    std::string outputFile;

    std::vector<std::string> createInitialRuns(Tape &inputTape);
    void mergeRuns(const std::vector<std::string> &runFiles, Tape &outputTape);
    void flushBuffer(Tape &outputTape, std::vector<Number> &buffer);
    void cleanup(const std::vector<std::string> &runFiles) const;

    unsigned long long allTapes = 0;
    unsigned long long currentTape = 0;

    // wymogi zadania
    void startMenu();
    void interMenu();
    int mergeCounter = 0;
    bool autoMerge = false;
    int mergesToPrintMenu = 0;
    void runPyScript();
    void enterData();
    void enterEntryFile();
    void printTapes();
};