#include "test_lib.h"
#include <thread>
#include <chrono>

extern "C" {
double calculate_sum_with_sleep(const int* arr, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
    return sum;
}
}