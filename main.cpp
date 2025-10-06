#include <string>
#include <iostream>
#include "Tape.h"
using namespace std;

const int tapeNumber = 4;
// liczba taśm wykorzystywanych do sortowania

int main()
{
    string var = "klasa.gray";
    cout << var.find(".") << endl;
    cout << var.substr(var.find(".")+1) << endl;
}