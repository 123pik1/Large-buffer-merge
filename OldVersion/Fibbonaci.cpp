#include "Fibbonaci.h"

int Fibbonaci::nextFibbo()
{
    int fibbo3 = fibbo1+fibbo2;
    fibbo1 = fibbo2;
    fibbo2 = fibbo3;
    return fibbo1;
}

int Fibbonaci::getSum()
{
    return fibbo1+fibbo2;
}

int Fibbonaci::get_n_InFuture(int n)
{
    if (n == 0)
        return fibbo1;
    if (n == 1)
        return fibbo2;

    int fibbo1_copy = fibbo1;
    int fibbo2_copy = fibbo2;

    for (int i = 2; i <= n; i++)
    {
        int fibbo3 = fibbo1_copy + fibbo2_copy;
        fibbo1_copy = fibbo2_copy;
        fibbo2_copy = fibbo3;
    }
    return fibbo2_copy;
}