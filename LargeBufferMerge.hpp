#pragma once

#include "constants.h"
#include "Tape.hpp"
#include <cstddef>
#include <string>
#include <vector>

class LargeBufferMerge
{
public:
    explicit LargeBufferMerge(std::size_t bufferSize = LARGE_BUFFER_SIZE, std::string inputFile=InputFile, std::string outputFile=OutputFile);
    ~LargeBufferMerge();
    void sort();

    std::size_t getReadCount() const noexcept { return readCount; }
    std::size_t getWriteCount() const noexcept { return writeCount; }

private:
    std::size_t bufferSize;
    std::size_t readCount;
    std::size_t writeCount;

    std::string inputFile;
    std::string outputFile;

    int numTapes = tapeNumber;
    std::vector<Tape*> runTapes;

    void createInitialRuns(Tape &inputTape);
    void mergeRuns(Tape &outputTape);
    void flushBuffer(Tape &outputTape, std::vector<Number> &buffer);
    void cleanup(const std::vector<std::string> &runFiles) const;

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