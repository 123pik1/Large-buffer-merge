#include "PolyphaseSort.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
PolyphaseSort::PolyphaseSort(int numTapes, const string &inputFile, const string &outputFile)
    : numTapes(numTapes), inputFile(inputFile), outputFile(outputFile)
{
    for (int i = 0; i < numTapes; ++i)
    {
        tapes.push_back(new Tape("tapes/tape" + to_string(i) + ".txt"));
    }
}

PolyphaseSort::~PolyphaseSort()
{
    for (Tape *tape : tapes)
    {
        string nmb = tape->getFilename();
        delete tape;
        remove(nmb.c_str());
    }
}

void PolyphaseSort::sort()
{
    // 1. Dystrybucja początkowa
    distributeInitialRuns();

    // 2. Merge na jedną taśmę
    while (!isSorted())
        mergePhase();
    // 3. Skopiowanie na taśmę outputu
    copyToOutput();
}

void PolyphaseSort::distributeInitialRuns()
{
    Tape inputTape(inputFile);
    int tapeIndex = 0;
    Number prev;
    bool first = true;

    int fibCurrent = 1;
    int fibNext = 1;
    int runCount = 0;

    while (!inputTape.isEmpty())
    {
        Number curr = inputTape.getCurrNumber();
        if (first || !(curr < prev))
        {
            tapes[tapeIndex]->appendNumber(curr);
            prev = curr;
            first = false;
        }
        else
        {

            tapes[tapeIndex]->appendNumber(curr);
            runCount++;
            prev = curr;
            first = false;
            if (runCount >= fibCurrent)
            {

                tapeIndex = (tapeIndex + 1) % (numTapes - 1);
                runCount = 0;

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
    for (Tape* tape:tapes)
        tape->printTape();
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
    vector<Tape *> sources;
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

    while (!sources[0]->isEmpty() && !sources[1]->isEmpty())
        mergeOneRun(destIndex, sources);
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

void PolyphaseSort::mergeOneRun(int destIndex, vector<Tape *> sources)
{
    Tape *destination = tapes[destIndex];

    

   priority_queue<pair<Number, int>, vector<pair<Number, int>>, greater<pair<Number, int>>> pq;

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
    while (!pq.empty())
    {
        auto [num, tapeIdx] = pq.top();
        pq.pop();
        destination->appendNumber(num);

        if (tapeIdx == 0)
        {
            sources[0]->readNextNumber();
            if (!sources[0]->isEmpty())
            {
                Number nextNum = sources[0]->getCurrNumber();
                if (nextNum > num)
                {
                    pq.push({nextNum, 0});
                }
            }
        }
        else
        {
            sources[1]->readNextNumber();
            if (!sources[1]->isEmpty())
            {
                Number nextNum = sources[1]->getCurrNumber();
                if (nextNum > num)
                {
                    pq.push({nextNum, 1});
                }
            }
        }
    }

    destination->writePage();
    destination->resetTape();
    sources[0]->deletePrevRecords();
    sources[0]->resetTape();
    sources[1]->deletePrevRecords();
    sources[1]->resetTape();
    // 4. Pętla w funkcji głownej powtarza póki więcej niż jedna taśma niepusta
}
