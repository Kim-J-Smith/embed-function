// Test moving / copying / invoking embed::function
#include <iostream>
#include <functional>
#include "embed_function.hpp"

#define GREEN "\033[32m"

#define RESET "\033[0m"

struct t2_001_multi_args_float_return
{
    float operator()(int a, int b, float c) const noexcept {
    std::cout << "t2_001_multi_args_float_return " << a << ' ' << b << ' ' << c << std::endl;
    return 1.11f;
    }

    t2_001_multi_args_float_return() = default;

    t2_001_multi_args_float_return(const t2_001_multi_args_float_return&) noexcept
    {
        std::cout << "Here is copy constructor" << std::endl;
    }

    t2_001_multi_args_float_return(t2_001_multi_args_float_return&&) noexcept
    {
        std::cout << "Here is move constructor" << std::endl;
    }

    void operator=(const t2_001_multi_args_float_return&) = delete;
};

struct t2_002_non_copyable_struct
{
    t2_002_non_copyable_struct(const t2_002_non_copyable_struct&) = delete;
    t2_002_non_copyable_struct(t2_002_non_copyable_struct&&) = default;
    t2_002_non_copyable_struct() = default;
    void operator=(const t2_002_non_copyable_struct&) = delete;
    ~t2_002_non_copyable_struct() = default;

    void operator()(int a) const noexcept {
        printf("Here is non-copyable struct, a=%d\n", a);
    }
};

void test_002()
{
    std::cout << "\n[START - test_002]\n" << std::endl;

    // Copy and move between embed::Fn

    auto fn1 = embed::make_function(t2_001_multi_args_float_return{});

    auto fn2 = embed::make_function(fn1);

    auto fn3 = fn1;

    auto fn4 = embed::make_function<void(int, int, float)>(fn1);

    auto fn5 = std::move(fn1);

    auto fn6 = embed::make_function<void()>();

    std::swap(fn1, fn5);

    fn5 = fn1;

    auto fn7 = embed::make_function<float(const int, int, float)>(fn1);

    std::cout << "fn7.buffer_size = " << fn7.buffer_size << std::endl;

    embed::function<float(const int, int, float), 32> fn8 = fn1;

    t2_001_multi_args_float_return t;

    auto fn9 = embed::make_function(t);

    auto fn10 = make_function(fn9); // ADL

    auto fn11 = embed::make_function(t2_002_non_copyable_struct{});

    auto fn12 = std::move(fn11);

    std::swap(fn11, fn12);

    fn12 = embed::make_function(t2_002_non_copyable_struct());

    embed::function<void(int)> fn13;

    fn13 = std::move(fn12);

    std::cout << "\n<test_002>: [BEGIN] Copy and move between embed::Fn" << std::endl;

    fn1(0, 0, 1);
    fn2(0, 0, 2);
    fn3(0, 0, 3);
    fn4(0, 0, 4);
    fn5(0, 0, 5);

#if EMBED_CXX_ENABLE_EXCEPTION
    try {
        fn6();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
# if defined(EMBED_NO_STD_HEADER)
    catch(const embed::_fn_no_std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
# endif
#endif

    fn7(0, 0, 7);
    fn8(0, 0, 8);
    fn9(0, 0, 9);
    fn10(0, 0, 10);
    fn11(11);
    fn13(13);

    std::cout << "<test_002>: [END] Copy and move between embed::Fn\n" << std::endl;

    std::cout << "[END - test_002] : " GREEN "OK" RESET "\n\n" << std::endl;
}

