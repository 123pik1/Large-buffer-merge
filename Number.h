#include <string>


// Class to contain number on tape
class Number
{
    std::string numberString;
    enum ComparisonResult
    {
        HIGHER,
        LOWER,
        EQUAL
    };

    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    ComparisonResult compareIntegers(std::string nmb1, std::string nmb2);
    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    ComparisonResult compareFloats(std::string nmb1, std::string nmb2);
    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    // Does not check the number of digits
    ComparisonResult compareDigitAfterDigit(std::string nmb1, std::string nmb2);

public:
    Number(std::string number);
    bool isInteger = false;
    bool isNegative = false;

    // If false nmb is higher, if true nmb is lower or equal
    bool isHigherThan(Number nmb);
    std::string getNumberString()
    {
        return numberString;
    }

    static std::string generateNumber(int nmbOfDigits);
};