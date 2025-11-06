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
    ComparisonResult compareIntegers(std::string nmb1, std::string nmb2) const;
    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    ComparisonResult compareFloats(std::string nmb1, std::string nmb2) const;
    // if nmb1 higher -> HIGHER, nmb2 higher -> LOWER, else EQUAL
    // Does not check the number of digits
    ComparisonResult compareDigitAfterDigit(std::string nmb1, std::string nmb2) const;
    void setInteger();
    void setNegative();
    ComparisonResult isWhatTo(Number nmb) const;
public:
    Number();
    Number(std::string number);
    bool isInteger = false;
    bool isNegative = false;

    // If false nmb is higher or equal, if true nmb is lower
    bool isHigherThan(Number nmb) const;
    bool isLowerThan(Number nmb) const;
    void setNumberString(std::string nmb);
    bool isEmpty() const
    {
        return numberString=="";
    }
    std::string getNumberString()
    {
        return numberString;
    }
    std::string to_string()
    {
        return numberString;
    }

    bool operator<(const Number& other) const
    {
        return this->isLowerThan(other);
    }
    bool operator>(const Number& other) const
    {
        return this->isHigherThan(other);
    }

    std::string to_string() const
    {
        return numberString;
    }
};