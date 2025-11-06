#include <iostream>
#include "PolyphaseSort.hpp"
using namespace std;

int main()
{
    // Number nmb1("-56921781675024989.2555453"), nmb2("-74871257181717776609032555743.6");
    // cout << (nmb1 < nmb2) << endl;
    PolyphaseSort algo(tapeNumber, InputFile,OutputFile);
    algo.sort();
    return 0;
}