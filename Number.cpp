#include "Number.h"
#include <vector>
#include <random>
using namespace std;

bool Number::isHigherThan(Number nmb)
{
    //! Add case with negatives
    if (!this->isInteger && nmb.isInteger)
        return false; // float is lower than integer
    if (this->isInteger && !nmb.isInteger)
        return true;
    if (this->isNegative && !nmb.isNegative)
        return false; // nmb not negative, this negative
    if (!this->isNegative && nmb.isNegative)
        return true;
    if (this->isInteger && nmb.isInteger)
    {
        Number::ComparisonResult res = compareIntegers(this->numberString, nmb.getNumberString());
        if (res == HIGHER)
        {
            if (this->isNegative)
                return false;
            return true;
        }
        if (res == LOWER)
        {
            if (this->isNegative)
                return true;
            return false;
        }
        return false;
    }
    Number::ComparisonResult res = compareFloats(this->numberString, nmb.getNumberString());
    if (res == HIGHER)
    {
        if (this->isNegative)
            return false;
        return true;
    }
    if (res==LOWER && this->isNegative)
        return true;
    return false;
}

Number::ComparisonResult Number::compareIntegers(string nmb1, string nmb2)
{
    if (nmb1.length() > nmb2.length())
        return HIGHER;
    if (nmb1.length() < nmb2.length())
        return LOWER;
    return compareDigitAfterDigit(nmb1, nmb2);
}

Number::ComparisonResult Number::compareFloats(string nmb1, string nmb2)
{
    string temp1, temp2;
    // Extracting substring from the beginning to "."
    int point1 = nmb1.find(".");
    int point2 = nmb2.find(".");
    temp1 = nmb1.substr(0, point1);
    temp2 = nmb2.substr(0, point2);

    ComparisonResult res = compareIntegers(temp1, temp2);
    if (res != EQUAL)
        return res;
    temp1 = nmb1.substr(point1 + 1);
    temp2 = nmb2.substr(point2 + 1);
    return compareDigitAfterDigit(temp1, temp2);
}

Number::ComparisonResult Number::compareDigitAfterDigit(string nmb1, string nmb2)
{
    char digit1, digit2;
    for (int i = 0; i < nmb1.length() && i<nmb2.length(); i++)
    {
        digit1 = nmb1.at(i);
        digit2 = nmb2.at(i);
        if (digit1 > digit2)
            return HIGHER;
        if (digit1 < digit2)
            return LOWER;
    }
    if (nmb1.length()>nmb2.length())
        return HIGHER;
    if (nmb1.length()<nmb2.length())
        return LOWER;
    return EQUAL;
}

Number::Number()
{
    setNumberString("");
}

Number::Number(string number)
{
    setNumberString(number);
}

void Number::setNumberString(string nmb)
{
    this->numberString = nmb;
    setInteger();
    setNegative();
}

void Number::setInteger()
{
    if (numberString.find(".") == string::npos)
        isInteger = true;
    else
        isInteger = false;
}

void Number::setNegative()
{
    if (numberString[0] == '-')
        isNegative = true;
    else
        isNegative = false;
}
