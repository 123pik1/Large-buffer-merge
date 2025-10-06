#include <string>
using namespace std;

// Class to contain number on tape
class Number
{
    string numberString;
    enum ComparisonResult
    {
        HIGHER,
        LOWER,
        EQUAL
    };

    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    ComparisonResult compareIntegers(string nmb1, string nmb2);
    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    ComparisonResult compareFloats(string nmb1, string nmb2);
    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    // Does not check the number of digits
    ComparisonResult compareDigitAfterDigit(string nmb1, string nmb2);

public:
    Number(string number);
    bool isInteger = false;

    // If false nmb is higher, if true nmb is lower or equal
    bool isHigherThan(Number nmb);
    string getNumberString()
    {
        return numberString;
    }
};