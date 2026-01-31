#include "my_fib_ext.h"
#include <iostream>

int main(){

    int n;
    std::cout << "Enter length of Fibonacci sequence: ";
    std::cin >> n;
    std::vector<int> fib_seq = fibonacci_sequence(n);
    std::cout << "Fibonacci sequence: ";
    for (int num : fib_seq){
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}