#include <iostream>
// #include "PolyphaseSort.hpp"
#include "Tape.hpp"
using namespace std;


int main()
{
    Tape input(InputFile);
    input.readNextNumber();

    cout << input.getCurrNumber().getNumberString()<<endl;
    input.printTape();
    input.readNextNumber();
    cout << input.getCurrNumber().getNumberString() << endl;

    Tape out(OutputFile);
    out.appendNumber(input.getCurrNumber());
    // PolyphaseSort poly;
    // poly.sort(InputFile, OutputFile);

    return 0;
}