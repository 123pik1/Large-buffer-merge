class Fibbonaci
{
    public:
    int fibbo1 = 1;
    int fibbo2 = 1;
    int nextFibbo();
    int prevFibbo();
    int getSum();
    // does not affect fibbo1 and fibbo2 but gets n times in future
    // 0 return fibbo1, 1 returns fibbo2, 2 sum, etc.
    int get_n_InFuture(int n);
};