#include "PolyphaseSort.hpp"
#include <iostream>
#include <algorithm>

PolyphaseSort::PolyphaseSort()
{
    // Create tapes (tapeNumber tapes for algorithm)
    for (int i = 0; i < tapeNumber; i++)
    {
        std::string filename = std::string(baseFileName) + std::to_string(i);
        tapes.push_back(new Tape(filename));
    }
    outputTapeIndex = tapeNumber - 1;
}

PolyphaseSort::~PolyphaseSort()
{
    for (auto tape : tapes)
    {
        delete tape;
    }
}

int PolyphaseSort::countRuns(Tape *tape)
{
    if (tape->isEmpty())
    {
        return 0;
    }

    int runs = 1;
    tape->goToBegin();
    tape->readNextNumber();
    Number prev = tape->getCurrNumber();

    while (!tape->isEmpty())
    {
        tape->readNextNumber();
        Number curr = tape->getCurrNumber();

        if (curr.isLowerThan(prev))
        {
            runs++;
        }
        prev = curr;
    }

    return runs;
}

void PolyphaseSort::distributeRuns(Tape *inputTape)
{
    // Distribute runs using Fibonacci-like pattern for polyphase
    int currentTape = 0;

    inputTape->goToBegin();

    while (!inputTape->isEmpty())
    {
        inputTape->readNextNumber();
        Number prev = inputTape->getCurrNumber();
        tapes[currentTape]->appendNumber(prev);

        // Continue current run
        while (!inputTape->isEmpty())
        {
            inputTape->readNextNumber();
            Number curr = inputTape->getCurrNumber();

            if (curr.isLowerThan(prev))
            {
                // New run starts, switch tape
                break;
            }

            tapes[currentTape]->appendNumber(curr);
            prev = curr;
        }

        tapes[currentTape]->writePage();

        // Round-robin to next input tape (skip output tape)
        currentTape = (currentTape + 1) % (tapeNumber - 1);
    }

    // Flush all tapes
    for (int i = 0; i < tapeNumber - 1; i++)
    {
        tapes[i]->writePage();
    }
}

int PolyphaseSort::findMinTape(std::vector<int> &activeTapes)
{
    int minTape = -1;
    Number minNumber;

    for (int tapeIdx : activeTapes)
    {
        if (minTape == -1)
        {
            minTape = tapeIdx;
            minNumber = tapes[tapeIdx]->getCurrNumber();
        }
        else
        {
            Number currNumber = tapes[tapeIdx]->getCurrNumber();
            if (currNumber.isLowerThan(minNumber))
            {
                minTape = tapeIdx;
                minNumber = currNumber;
            }
        }
    }

    return minTape;
}

int PolyphaseSort::mergePhase()
{
    // Prepare input tapes (all except output)
    std::vector<int> inputTapes;
    for (int i = 0; i < tapeNumber; i++)
    {
        if (i != outputTapeIndex)
        {
            tapes[i]->goToBegin();
            if (!tapes[i]->isEmpty())
            {
                tapes[i]->readNextNumber();
                inputTapes.push_back(i);
            }
        }
    }

    if (inputTapes.empty())
    {
        return 0;
    }

    int mergedRuns = 0;

    // Merge until all input tapes are empty
    while (!inputTapes.empty())
    {
        std::vector<int> activeTapes = inputTapes;
        std::vector<Number> lastValues(tapeNumber);

        // Merge one run from each tape
        while (!activeTapes.empty())
        {
            int minTapeIdx = findMinTape(activeTapes);
            Number minNumber = tapes[minTapeIdx]->getCurrNumber();

            tapes[outputTapeIndex]->appendNumber(minNumber);
            lastValues[minTapeIdx] = minNumber;

            // Read next from min tape
            if (!tapes[minTapeIdx]->isEmpty())
            {
                tapes[minTapeIdx]->readNextNumber();
                Number nextNumber = tapes[minTapeIdx]->getCurrNumber();

                // Check if run ended
                if (nextNumber.isLowerThan(minNumber))
                {
                    activeTapes.erase(
                        std::remove(activeTapes.begin(), activeTapes.end(), minTapeIdx),
                        activeTapes.end());
                }
            }
            else
            {
                activeTapes.erase(
                    std::remove(activeTapes.begin(), activeTapes.end(), minTapeIdx),
                    activeTapes.end());
                inputTapes.erase(
                    std::remove(inputTapes.begin(), inputTapes.end(), minTapeIdx),
                    inputTapes.end());
            }
        }

        mergedRuns++;
    }

    tapes[outputTapeIndex]->writePage();
    return mergedRuns;
}

void PolyphaseSort::sort(const std::string &input, const std::string &output)
{
    // Load input
    Tape *inputTape = new Tape(input);

    std::cout << "Distributing initial runs..." << std::endl;
    distributeRuns(inputTape);
    delete inputTape;

    // Print initial distribution
    for (int i = 0; i < tapeNumber - 1; i++)
    {
        std::cout << "Tape " << i << " runs: " << countRuns(tapes[i]) << std::endl;
    }

    // Polyphase merge
    int phase = 0;
    while (true)
    {
        std::cout << "\nPhase " << ++phase << std::endl;

        int runs = mergePhase();
        std::cout << "Merged " << runs << " run(s) to tape " << outputTapeIndex << std::endl;

        if (runs <= 1)
        {
            break;
        }

        // Clear input tapes and rotate
        for (int i = 0; i < tapeNumber; i++)
        {
            if (i != outputTapeIndex)
            {
                tapes[i]->resetTape();
                tapes[i]->deletePrevRecords();
            }
        }

        // Rotate output tape
        int newOutputIndex = (outputTapeIndex + 1) % tapeNumber;
        outputTapeIndex = newOutputIndex;
    }

    std::cout << "\nSorting complete! Result on tape " << outputTapeIndex << std::endl;

    // Copy result to output file
    Tape *outputTape = new Tape(output);
    tapes[outputTapeIndex]->goToBegin();

    while (!tapes[outputTapeIndex]->isEmpty())
    {
        tapes[outputTapeIndex]->readNextNumber();
        outputTape->appendNumber(tapes[outputTapeIndex]->getCurrNumber());
    }
    outputTape->writePage();

    delete outputTape;
}