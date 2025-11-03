#include <iostream>
#include "PolyphaseSort.hpp"
using namespace std;


int main()
{
    PolyphaseSort algo(tapeNumber, InputFile,OutputFile);
    algo.sort();
    return 0;
}