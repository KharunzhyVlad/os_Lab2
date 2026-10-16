#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <dlfcn.h>

std::vector<int> g_array;
int g_min_val = 0;
int g_max_val = 0;
double g_avg_val = 0.0;

void thread_min_max() {
    if (g_array.empty()) return;

    int mn = g_array[0];
    int mx = g_array[0];

    for (size_t i = 1; i < g_array.size(); ++i) {
        if (g_array[i] < mn) {
            mn = g_array[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));

        if (g_array[i] > mx) {
            mx = g_array[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    g_min_val = mn;
    g_max_val = mx;
    std::cout << "[Поток min_max] Минимум: " << g_min_val << ", Максимум: " << g_max_val << std::endl;
}

typedef double (*SumFunc)(const int*, int);

void thread_average() {
    if (g_array.empty()) return;

    void* hLib = dlopen("libtest_lib.dylib", RTLD_LAZY);
    if (!hLib) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
        return;
    }

    SumFunc sum_func = (SumFunc)dlsym(hLib, "calculate_sum_with_sleep");
    if (!sum_func) {
        std::cerr << "Ошибка поиска функции: " << dlerror() << std::endl;
        dlclose(hLib);
        return;
    }

    double total_sum = sum_func(g_array.data(), g_array.size());
    g_avg_val = total_sum / g_array.size();

    dlclose(hLib);

    std::cout << "[Поток average] Среднее арифметическое: " << g_avg_val << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cout << "Введите размер массива: ";
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Неверный размер!" << std::endl;
        return 1;
    }

    g_array.resize(n);
    std::cout << "Введите элементы массива через пробел: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> g_array[i];
    }

    std::thread t1(thread_min_max);
    std::thread t2(thread_average);

    t1.join();
    t2.join();

    int replacement = static_cast<int>(g_avg_val);
    for (int i = 0; i < n; ++i) {
        std::cout << "Ищем совпадения..." << std::endl;
        if (g_array[i] == g_min_val || g_array[i] == g_max_val) {
            g_array[i] = replacement;
        }
    }

    std::cout << "Результирующий массив: ";
    for (int i = 0; i < n; ++i) {
        std::cout << g_array[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}