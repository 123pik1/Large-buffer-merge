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
    if (this->isInteger && nmb.isInteger)
    {
        Number::ComparisonResult res = compareIntegers(this->numberString, nmb.getNumberString());
        if (res == HIGHER)
            return true;
        if (res == LOWER)
            return false;
    }
    Number::ComparisonResult res = compareFloats(this->numberString, nmb.getNumberString());
    if (res ==LOWER)
        return false;
    return true;
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
    temp1 = nmb1.substr(0,point1);
    temp2 = nmb2.substr(0,point2);

    ComparisonResult res = compareIntegers(temp1, temp2);
    if (res != EQUAL)
        return res;
    temp1 = nmb1.substr(point1+1);
    temp2 = nmb2.substr(point2+1);
    return compareDigitAfterDigit(temp1, temp2);
}


Number::ComparisonResult Number::compareDigitAfterDigit(string nmb1, string nmb2)
{
    char digit1, digit2;
    for (int i = 0; i < nmb1.length(); i++)
    {
        digit1 = nmb1.at(i);
        digit2 = nmb2.at(i);
        if (digit1 > digit2)
            return HIGHER;
        if (digit1 < digit2)
            return LOWER;
    }
    return EQUAL;
}

string Number::generateNumber(int nmbOfDigits)
{
    string number = "";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> digitDist(0,9);
    uniform_int_distribution<int> typeDist(0,1);

    bool makeNegative = (typeDist(gen) == 1);
    if (makeNegative)
    {
        number+="-";
    }

    bool makeFloat = (typeDist(gen) == 1 && nmbOfDigits>1);
    int decimalPos = -1; //no decimal pos if makefloat false
    if(makeFloat)
    {
        uniform_int_distribution<int> decPosDist(1,nmbOfDigits);
        decimalPos = decPosDist(gen);
    }
    if (decimalPos==1)
    {
        number += to_string(digitDist(gen));
        // if decimal is after first digit it can be 0
    }
    else
    {
        uniform_int_distribution<int> firstDigit(1, 9);
        number += to_string(firstDigit(gen));
    }

    
    for (int i=1; i<nmbOfDigits; i++)
    {
        if (i==decimalPos)
        {
            number += ".";
        }
        number += to_string(digitDist(gen));
    }

    return number;
}