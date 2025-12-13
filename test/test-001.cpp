#include <iostream>
#include "embed_function.hpp"

struct test_struct
{
    int i;
    float f;
};

void print_hello() { std::cout << "hello" << std::endl; }

int print_world(int a, int b, float c) {

    std::cout << "world " << a << ' ' << b << ' ' << c << std::endl;
    return 1;
}

void print_struct(const test_struct& s)
{
    std::cout << "i = " << s.i << " f = " << s.f << std::endl;
}

void test_001()
{
    embed::function<void()> fn1 = print_hello;

    embed::function<int(int, int, float)> fn2 = print_world;

    // Ret: int --> void
    embed::function<void(int, int, float)> fn3 = print_world;

    // Arg: float --> int
    embed::function<int(int, int, int)> fn4 = print_world;

    embed::function<void(const test_struct&)> fn5 = print_struct;

    int ret = 0;
    auto s = test_struct{11445, 3.14159};

    fn1();
    ret = fn2(1, 2, 3.0f);
    fn3(4, 5, 6.0f);
    ret = fn4(7, 8, 9);
    fn5(s);

    std::cout << "ret = " << ret << std::endl;
    
    std::cout << "test - 001 finish" << std::endl;
}
