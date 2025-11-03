#include <iostream>
// #include "PolyphaseSort.hpp"
#include "Tape.hpp"
using namespace std;


int main()
{
    Tape input(InputFile);
    input.readNextNumber();

    cout << input.getCurrNumber().getNumberString()<<endl;
    input.readNextNumber();
    cout << input.getCurrNumber().getNumberString() << endl;

    Tape out(OutputFile);
    out.clearTape();
    out.appendNumber(input.getCurrNumber());
    input.deletePrevRecords();



    // out.printTape();
    // PolyphaseSort poly;
    // poly.sort(InputFile, OutputFile);

    return 0;
}