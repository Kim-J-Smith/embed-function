#include <iostream>
#include <functional>
#include "embed_function.hpp"

static float print_world(int a, int b, float c) {

    std::cout << "world " << a << ' ' << b << ' ' << c << std::endl;
    return 1.11;
}

void test_002()
{
    embed::function<int(int, int, float)> fn1 = print_world;

    // function<A> -> function<B>
    embed::function<int(int, int, const float)> fn2 = fn1;

    embed::function<int(int, const int, const float)> fn3 = std::move(fn2);

    embed::function<void(int, long, double), 16> fn4 = fn3;

    int ret = fn1(1, 2, 3.14f);
    fn3(1, 2, 4.6677);

    std::swap(fn2, fn3);
    fn2(1, 4, 5.0f);
    fn4(1, 2, 3.1415926535);

    try{
        fn3(99, 98, 97.666);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "ret = " << ret << std::endl;
    std::cout << "test - 002 finish" << std::endl;
}

