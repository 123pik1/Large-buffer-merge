#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Tape.h"
#include "Fibbonaci.h"
using namespace std;

// liczba taśm wykorzystywanych do sortowania
// liczba taśm nie mniejsza od 3
#define tapeNumber 3
// lokalizacja pliku z którego jest wczytywanie
#define inputFile "data/exampleData"
// bazowa nazwa pliku z taśmą
#define baseFileName "tapes/tape"
// lokalizacja pliku do którego jest zapisany wynik
#define outputFile "output/result"

// zlicza dostępy do pamięci
int memoryAccessCounter = 0;
// zlicza fazy sortowania
int sortPhasesCounter = 0;

//TODO
/*TODO
- proper memory access counter
- sort phase counter
- menu użytkownika
*/

void merging(Tape **tapes, int idEmpty)
{
    while (true)
    {
        // znalezienie najmniejszej liczby
        int idLowest = 0;
        for (int i = 0; i < tapeNumber; i++)
        {
            if (i == idEmpty)
                continue;
            if (tapes[i]->isEmpty())
                return;
            if (i == idLowest) //? optional - możliwe do usunięcia
                continue;
            if (tapes[idLowest]->getCurrentNumber().isHigherThan(tapes[i]->getCurrentNumber()))
            {
                idLowest = i;
            }
        }

        // dodanie liczby do pustej taśmy
        tapes[idEmpty]->appendNumber(tapes[idLowest]->getCurrentNumber());
        tapes[idLowest]->readNextNumber();
    }
}

// returns how much there are empty tapes
int sortIteration(Tape **tapes)
{
    cout<<"iteracja sortowania"<<endl;
    // znalezienie pustej taśmy
    int idEmpty = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        cout<<"for nr 1 w sortIteration"<<endl;
        if (tapes[i]->isEmpty())
        {
            idEmpty = i;
            break;
        }
        cout << "for nr 1 w sortIteration 2" << endl;
    }
    cout<<"pusta taśma znaleziona";
    // etap scalania taśm
    merging(tapes, idEmpty);
    cout<<"taśmy zmergowane";
    // zlicza puste taśmy
    int emptyCount = 0;
    for (int i = 0; i < tapeNumber; i++)
    {
        if (tapes[i]->isEmpty())
            emptyCount++;
    }
    cout << ++sortPhasesCounter;
    return emptyCount;
}

// zwraca id niepustej taśmy
int sort(Tape **tapes)
{
    cout<<"początek sortowania"<<endl;
    // sortuje póki jest więcej niż jedna niepusta taśma
    while (tapeNumber - sortIteration(tapes) > 1)
        ;

    // sprawdza która taśma jest niepusta
    for (int i = 0; i < tapeNumber; i++)
    {
        if (!tapes[i]->isEmpty())
            return i;
    }
    return 0;
}

int countSeries()
{
    Number actualNmb, previousNmb;
    Tape mainFile(inputFile);
    int series = 1;
    mainFile.readNextNumber();
    actualNmb = mainFile.getCurrentNumber();

    do
    {
        previousNmb = actualNmb;
        mainFile.readNextNumber();
        if (mainFile.getCurrentNumber().getNumberString() != "")
            break;
        actualNmb = mainFile.getCurrentNumber();
        // when previous is higher then adds series
        //? possible place to check
        if (!actualNmb.isHigherThan(previousNmb))
            series++;
    } while (mainFile.getCurrentNumber().getNumberString() != "");

    return series;
}

void parseInputFile(Tape **tapes)
{
    Number previousNmb, currentNumber;
    int tapeID = 0;
    int totalSeries = countSeries();

    if (totalSeries == 0)
        return;

    Fibbonaci fibbo;
    int sum = 0;
    while (sum < totalSeries)
    {
        sum = 0;
        for (int i = 0; i < tapeNumber; i++)
        {
            sum += fibbo.get_n_InFuture(i);
        }
        if (sum < totalSeries)
            fibbo.nextFibbo();
    }

    int distribution[tapeNumber];
    distribution[0] = fibbo.fibbo1;
    distribution[1] = fibbo.fibbo2;

    for (int i = 2; i < tapeNumber - 1; i++)
    {
        distribution[i] = fibbo.get_n_InFuture(i);
    }
    distribution[tapeNumber - 1] = 0;

    Tape inputTape(inputFile);
    // inputTape.printTape(); //! wyświetlanie pliku przed sortowaniem
    cout << "fibbonaci zrobiony" << endl;
    inputTape.readNextNumber();
    cout << "przeczytało pierwszy numer: " << inputTape.getCurrentNumber().getNumberString() << endl;
    if (inputTape.getCurrentNumber().getNumberString() == "")
        return;
    cout << "pominelo ifa" << endl;
    previousNmb = inputTape.getCurrentNumber();
    int currentTapeId = 0;
    int seriesOnCurrentTape = 0;
    cout << "inicjalizacja intow i previousnmb" << endl;

    tapes[currentTapeId]->appendNumber(previousNmb);
    cout << "appenduje pierwsza liczbe" << endl;

    while (true)
    {
        inputTape.readNextNumber();
        cout << inputTape.file.tellg() << endl;
        cout <<"Nowa liczba: "<< inputTape.getCurrentNumber().getNumberString() << endl;
        if (inputTape.getCurrentNumber().getNumberString()=="")
            break;
        cout << "Ta liczba przeszla dalej: " << inputTape.getCurrentNumber().getNumberString() << endl;
        currentNumber = inputTape.getCurrentNumber();

        if (!currentNumber.isHigherThan(previousNmb))
        {
            seriesOnCurrentTape++;

            // when higher or equal because series are counted from 0
            if (seriesOnCurrentTape >= distribution[currentTapeId])
            {
                do
                {
                    currentTapeId = (currentTapeId + 1) % (tapeNumber - 1);
                } while (distribution[currentTapeId] == 0 && currentTapeId < tapeNumber - 1);

                seriesOnCurrentTape = 0;
            }
        }

        tapes[currentTapeId]->appendNumber(currentNumber);
        previousNmb = currentNumber;
        
    }
    cout<<"wyjście z while true nr 1"<<endl;
    for (int i = 0; i < tapeNumber; i++)
        tapes[i]->resetToBeginning();
    cout<<"taśmy zresetowane"<<endl;
}

void prepareTapes(Tape **tapes)
{
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i]->clearTape();
    }
    parseInputFile(tapes);
}

int main()
{
    Tape *tapes[tapeNumber];
    for (int i = 0; i < tapeNumber; i++)
    {
        tapes[i] = new Tape(baseFileName + to_string(i));
        tapes[i]->clearTape();
    }
    prepareTapes(tapes);
    int id = sort(tapes);
    Tape outputTape(outputFile);
    tapes[id]->copyTapeTo(&outputTape);
}