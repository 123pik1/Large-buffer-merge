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

    // Number nmb1("10679022"), nmb2("-151920.8");
    // cout << (nmb1<nmb2)<<endl;
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
            tapes[tapeIndex]->appendNumber(curr);
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
    // for (Tape* tape:tapes)
    //     tape->printTape();
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
    // 2. Znalezienie wszystkich niepustych taśm
    vector<Tape *> sources;
    for (int i = 0; i < numTapes; ++i)
    {
        if (i != destIndex && !tapes[i]->isEmpty())
        {
            sources.push_back(tapes[i]);
        }
    }
    if (sources.empty())
    {
        return;
    }

    // Powtarzaj merge run po runie póki jedna source taśma nie spuścieje
    while (true)
    {
        bool canMerge = true;
        for (auto s : sources)
        {
            if (s->isEmpty())
            {
                canMerge = false;
                break;
            }
        }
        if (!canMerge)
            break;
        mergeOneRun(destIndex, sources);
        // cout << "After mergeOneRun:" << endl;
        // for (Tape *tape : tapes)
        //     tape->printTape();
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

void PolyphaseSort::mergeOneRun(int destIndex, vector<Tape *> sources)
{
    Tape *destination = tapes[destIndex];
    priority_queue<pair<Number, int>, vector<pair<Number, int>>, greater<pair<Number, int>>> pq;
    vector<Number> lastFromSource(sources.size());
    vector<bool> runActive(sources.size(), true);

    for (int i = 0; i < sources.size(); i++)
    {
        if (!sources[i]->isEmpty())
        {
            Number num = sources[i]->getCurrNumber();
            pq.push({num, i});
            lastFromSource[i] = num;
        }
    }

    while (!pq.empty())
    {
        auto [num, sourceId] = pq.top();
        pq.pop();
        destination->appendNumber(num);

        sources[sourceId]->readNextNumber();
        if (!sources[sourceId]->isEmpty())
        {
            Number nextNum = sources[sourceId]->getCurrNumber();
            if (nextNum < lastFromSource[sourceId])
            {
                // KONIEC RUNU na tej taśmie
                runActive[sourceId] = false;
            }
            else
            {
                pq.push({nextNum, sourceId});
                lastFromSource[sourceId] = nextNum;
            }
        }
        else
        {
            runActive[sourceId] = false;
        }

        // jeśli wszystkie runy są zakończone -> przerwij ten merge
        if (all_of(runActive.begin(), runActive.end(), [](bool active)
                   { return !active; }))
        {
            break;
        }
    }

    destination->writePage();
}

//TODO result is not sorted properly