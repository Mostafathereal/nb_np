#include "my_fib_ext.h"

std::vector<int> fibonacci_sequence(int n){
    std::vector<int> fib_seq;

    if (n <=0) return fib_seq;
    fib_seq.push_back(0);
    if (n == 1) return fib_seq;
    fib_seq.push_back(1);

    for (int i = 2; i < n; i++){
        fib_seq.push_back(fib_seq[i-1] + fib_seq[i-2]);
    }

    return fib_seq;

}