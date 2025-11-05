#include <iostream>
#include "PolyphaseSort.hpp"
using namespace std;

int main()
{
    // Number nmb1("906745078"), nmb2("-645740.2");
    // cout << (nmb1 < nmb2) << endl;
    PolyphaseSort algo(tapeNumber, InputFile,OutputFile);
    algo.sort();
    return 0;
}