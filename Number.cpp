#include "Number.hpp"
#include <vector>
#include <random>
using namespace std;

// priorytety:
// 1. float mniejszy od inta
// 2. ujemne mniejsze od dodatnich
// 3. Porównanie matematyczne
Number::ComparisonResult Number::isWhatTo(Number nmb) const
{
    if (!this->isInteger && nmb.isInteger)
        return LOWER; // float is lower than integer
    if (this->isInteger && !nmb.isInteger)
        return HIGHER;

    if (this->isNegative && !nmb.isNegative)
        return LOWER; // nmb not negative, this negative
    if (!this->isNegative && nmb.isNegative)
        return HIGHER;
    ComparisonResult res;
    if (this->isInteger && nmb.isInteger)
        res = compareIntegers(this->numberString, nmb.getNumberString());
    else
        res = compareFloats(this->numberString, nmb.getNumberString());

    if (res == EQUAL)
        return EQUAL;

    if (this->isNegative)
        return (res == HIGHER) ? LOWER : HIGHER;
    else
        return res;
}

bool Number::isHigherThan(Number nmb) const
{
    return (isWhatTo(nmb) == HIGHER);
}

bool Number::isLowerThan(Number nmb) const
{
    return (isWhatTo(nmb) == LOWER);
}

Number::ComparisonResult Number::compareIntegers(string nmb1, string nmb2) const
{
    string clean1 = (nmb1[0] == '-') ? nmb1.substr(1) : nmb1;
    string clean2 = (nmb2[0] == '-') ? nmb2.substr(1) : nmb2;
    if (clean1.length() > clean2.length())
        return HIGHER;
    if (clean1.length() < clean2.length())
        return LOWER;
    return compareDigitAfterDigit(clean1, clean2);
}

Number::ComparisonResult Number::compareFloats(string nmb1, string nmb2) const
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

Number::ComparisonResult Number::compareDigitAfterDigit(string nmb1, string nmb2) const
{
    char digit1, digit2;
    for (int i = 0; i < nmb1.length() && i < nmb2.length(); i++)
    {
        digit1 = nmb1.at(i);
        digit2 = nmb2.at(i);
        if (digit1 > digit2)
            return HIGHER;
        if (digit1 < digit2)
            return LOWER;
    }
    if (nmb1.length() > nmb2.length())
        return HIGHER;
    if (nmb1.length() < nmb2.length())
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
    if (nmb.empty())
    {
        numberString = "";
        return;
    }
    // Normalize: remove leading zeros, handle sign and decimal
    bool negative = (nmb[0] == '-');
    std::string absNmb = negative ? nmb.substr(1) : nmb;
    size_t dotPos = absNmb.find('.');
    if (dotPos != std::string::npos)
    {
        // Remove trailing zeros in fractional part
        size_t lastNonZero = absNmb.find_last_not_of('0');
        if (lastNonZero > dotPos)
        {
            absNmb = absNmb.substr(0, lastNonZero + 1);
        }
        else
        {
            absNmb = absNmb.substr(0, dotPos); // Remove decimal if no fractional
        }
    }
    // Remove leading zeros from integer part
    size_t start = absNmb.find_first_not_of('0');
    if (start != std::string::npos)
    {
        absNmb = absNmb.substr(start);
    }
    else
    {
        absNmb = "0";
    }
    numberString = negative ? "-" + absNmb : absNmb;
    if (numberString == "-0")
        numberString = "0";
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
    if (!numberString.empty() && numberString[0] == '-')
        isNegative = true;
    else
        isNegative = false;
}
