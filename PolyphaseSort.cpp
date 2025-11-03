#include "PolyphaseSort.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

PolyphaseSort::PolyphaseSort(int numTapes, const std::string &inputFile, const std::string &outputFile)
    : numTapes(numTapes), inputFile(inputFile), outputFile(outputFile)
{
    for (int i = 0; i < numTapes; ++i)
    {
        tapes.push_back(new Tape("tapes/tape" + std::to_string(i) + ".txt"));
    }
}

PolyphaseSort::~PolyphaseSort()
{
    for (Tape *tape : tapes)
    {
        string nmb = tape->getFilename();
        delete tape;
        // remove(nmb.c_str());
    }
}

void PolyphaseSort::sort()
{
    // Step 1: Distribute initial runs to tapes
    distributeInitialRuns();

    // Step 2: Merge all to one tape
    // while (!isSorted())
        mergePhase();

    // Step 3: Copy the sorted data to output file
    copyToOutput();
}

void PolyphaseSort::distributeInitialRuns()
{
    Tape inputTape(inputFile);
    int tapeIndex = 0;
    Number prev;
    bool first = true;

    // Iterative Fibonacci for run distribution (no pre-defined vector)
    int fibCurrent = 1; // Current Fibonacci number for run limit
    int fibNext = 1;    // Next Fibonacci number
    int runCount = 0;   // Runs assigned to current tape

    while (!inputTape.isEmpty())
    {
        Number curr = inputTape.getCurrNumber();
        if (first || !(curr < prev))
        { // curr >= prev, continue run
            tapes[tapeIndex]->appendNumber(curr);
            prev = curr;
            first = false;
        }
        else
        {
            // Start new run: assign to current tape, increment run count
            tapes[tapeIndex]->appendNumber(curr);
            runCount++;
            prev = curr;
            first = false;
            // Check if we've reached the Fibonacci limit for this tape
            if (runCount >= fibCurrent)
            {
                // Switch to next tape (leaving one empty)
                tapeIndex = (tapeIndex + 1) % (numTapes - 1);
                runCount = 0;
                // Update Fibonacci numbers iteratively
                int temp = fibCurrent;
                fibCurrent = fibNext;
                fibNext = temp + fibNext;
            }
        }
        inputTape.readNextNumber();
    }

    for (auto tape : tapes)
    {
        tape->writePage();
        // tape -> printTape();
    }
}

void PolyphaseSort::mergePhase()
{
    // 1. Znalezienie pustej taśmy
    int destIndex = findEmpty();
    if (destIndex == -1)
    {
        cout << "pusta nieznaleziona" << endl;
        return;
    }
    cout << "Pusta " << destIndex << endl;
    Tape *destination = tapes[destIndex];
    destination->clearTape();

    // 2. Znalezienie dwóch niepustych taśm
    std::vector<Tape *> sources;
    for (int i = 0; i < numTapes; ++i)
    {
        if (i != destIndex && !tapes[i]->isEmpty())
        {
            sources.push_back(tapes[i]);
            if (sources.size() == 2)
                break;
        }
    }
    if (sources.size() < 2)
    {
        return;
    }

    std::priority_queue<std::pair<Number, int>, std::vector<std::pair<Number, int>>, std::greater<std::pair<Number, int>>> pq;

    for (int i = 0; i < sources.size(); ++i)
    {
        sources[i]->goToBegin();
        if (!sources[i]->isEmpty())
        {
            Number num = sources[i]->getCurrNumber();
            pq.push({num, i});
        }
    }

    // 2. Merge dwóch serii z sourców do destynacji
    // 3. Powtórz póki na jedna z nich nie będzie pusta
    bool source0Empty = false;
    bool source1Empty = false;
    while (!pq.empty() && !(source0Empty || source1Empty))
    {
        auto [num, tapeIdx] = pq.top();
        pq.pop();
        destination->appendNumber(num);

        if (tapeIdx == 0)
        {
            sources[0]->readNextNumber();
            if (sources[0]->isEmpty())
            {
                source0Empty = true;
            }
            else
            {
                Number nextNum = sources[0]->getCurrNumber();
                pq.push({nextNum, 0});
            }
        }
        else
        {
            sources[1]->readNextNumber();
            if (sources[1]->isEmpty())
            {
                source1Empty = true;
            }
            else
            {
                Number nextNum = sources[1]->getCurrNumber();
                pq.push({nextNum, 1});
            }
        }
    }

    destination->writePage();

    for (auto source : sources)
    {
        source->printTape();
        source->deletePrevRecords();
    }
    // 4. Pętla w funkcji głownej powtarza póki więcej niż jedna taśma niepusta
}

bool PolyphaseSort::isSorted()
{
    int nonEmptyCount = 0;
    for (auto tape : tapes)
    {
        if (!tape->isEmpty())
        {
            nonEmptyCount++;
        }
    }
    return nonEmptyCount <= 1;
}

void PolyphaseSort::copyToOutput()
{
    Tape *sortedTape = nullptr;
    for (auto tape : tapes)
    {
        if (!tape->isEmpty())
        {
            sortedTape = tape;
            break;
        }
    }

    if (sortedTape)
    {
        Tape outputTape(outputFile);
        outputTape.clearTape();
        sortedTape->goToBegin();
        while (!sortedTape->isEmpty())
        {
            Number num = sortedTape->getCurrNumber();
            outputTape.appendNumber(num);
            sortedTape->readNextNumber();
        }
        outputTape.writePage();
    }
}

int PolyphaseSort::findEmpty()
{
    int destIndex = -1;
    for (int i = 0; i < numTapes; ++i)
    {
        if (tapes[i]->isEmpty())
        {
            destIndex = i;
            break;
        }
    }
    return destIndex;
}