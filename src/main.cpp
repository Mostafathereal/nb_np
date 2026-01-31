#include "math_core.h"
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

    double arr[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    size_t count = sizeof(arr) / sizeof(arr[0]);
    double_values(arr, count);
    std::cout << "Doubled values: ";
    for (size_t i = 0; i < count; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}