#include <string>
using namespace std;

// Class to contain number on tape
class Number
{
    bool isInteger = false;
    string numberString;

    public:
    // If false nmb is higher, if true nmb is lower
    bool isHigherThan(Number nmb);
    string getNumberString()
    {
        return numberString;
    }
};