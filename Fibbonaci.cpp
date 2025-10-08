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
    int fibbo1_copy = fibbo1;
    int fibbo2_copy = fibbo2;
    int fibbo3 = fibbo1+fibbo2;
    
    for (int i=0; i<n; i++)
    {
        fibbo1_copy = fibbo2_copy;
        fibbo2_copy = fibbo3;
        fibbo3 = fibbo1_copy+fibbo2_copy;
    }
    return fibbo1_copy;
}