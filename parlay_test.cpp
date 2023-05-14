#include <iostream>
#include "parallel.h"

using namespace std;

void foo()
{
    cout << "foo" << endl;
}

void bar()
{
    cout << "bar" << endl;
}

int main()
{
    int n = 5;
    int *arr = (int *)malloc(n * sizeof(int));

    parlay::parallel_for(0, n, [&](size_t i)
                         { arr[i] = i; });

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << "\n";
    }

    for (int i = 0; i < 50; i++)
    {
        parlay::par_do(foo, bar, false);
    }
}