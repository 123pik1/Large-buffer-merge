#include <iostream>
#include "PolyphaseSort.hpp"
using namespace std;

int main()
{

    // Number nmb1("93452027"), nmb2("-151920.8");
    // cout << (nmb1 < nmb2) << endl;
    PolyphaseSort algo(tapeNumber, InputFile,OutputFile);
    algo.sort();
    return 0;
}