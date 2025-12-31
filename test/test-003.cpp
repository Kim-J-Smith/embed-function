#include <iostream>
#include <chrono>
#include "embed/embed_function.hpp"

#define TIME_(x) (std::chrono::duration_cast<std::chrono::nanoseconds>(x).count()) << " ns"

void t3_normal_function(int a, float b) {
    volatile float i = (float)a + b;
    (void)i;
}

void test_003()
{
    std::cout << "\n[START - test_003]\n" << std::endl;

    auto time_begin_1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000000; i++)
        volatile embed::function<void(int, float)> fn1 = t3_normal_function;

    auto time_end_1 = std::chrono::high_resolution_clock::now();

    std::cout << "time_constructor_normal_func = " << TIME_(time_end_1-time_begin_1) << std::endl;

    auto time_begin_2 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000000; i++)
        volatile embed::function<void(int, float)> fn2 = [](int a, float b){ t3_normal_function(a,b); };

    auto time_end_2 = std::chrono::high_resolution_clock::now();

    std::cout << "time_constructor_lambda_func = " << TIME_(time_end_2-time_begin_2) << std::endl;


    embed::function<void(int, float)> fn1 = t3_normal_function;
    embed::function<void(int, float)> fn2 = [](int a, float b){ t3_normal_function(a,b); };

    auto time_begin_3 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000000; i++)
        std::swap(fn1, fn2);

    auto time_end_3 = std::chrono::high_resolution_clock::now();

    std::cout << "time_swap_func = " << TIME_(time_end_3-time_begin_3) << std::endl;

    auto time_begin_4 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000000; i++) {
        embed::function<void(int, float)> fn3;
        fn3 = [](int a, float b){ t3_normal_function(a,b); };
    }

    auto time_end_4 = std::chrono::high_resolution_clock::now();

    std::cout << "time_constructor_assign = " << TIME_(time_end_4-time_begin_4) << std::endl;

    auto time_begin_5 = std::chrono::high_resolution_clock::now();

    embed::function<void(int, float)> fn4 = t3_normal_function;
    for (int i = 0; i < 100000000; i++) {
        fn4(i, (float)(i+1));
    }

    auto time_end_5 = std::chrono::high_resolution_clock::now();

    std::cout << "time_invoke = " << TIME_(time_end_5-time_begin_5) << std::endl;

    std::cout << "[END - test_003] : ------ "  "OK"  " ------\n\n" << std::endl;
}


