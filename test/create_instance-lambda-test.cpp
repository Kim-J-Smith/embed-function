/**
 * Here is the test for constructors and `make_function` (lambda function).
 */
#include "embed/embed_function.hpp"
#include "test.hpp"

TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Basic_NonArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Basic_SingleArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Basic_MultiSameTypeArgs);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Basic_MultiDiffTypeArgs);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Capture_Value);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Capture_Reference);

TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, make_function_NonArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, make_function_SingleArg);

TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Mutable_Lambda);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Noexcept_Lambda);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Return_Void);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Constexpr_Lambda);
TEST_FUNCTION_DECLARE(CreateInstanceFromLambda, Nested_Lambda);

TEST_SUBSYS(CreateInstanceFromLambda, main) {
    TEST_RUN(CreateInstanceFromLambda, Basic_NonArg);
    TEST_RUN(CreateInstanceFromLambda, Basic_SingleArg);
    TEST_RUN(CreateInstanceFromLambda, Basic_MultiSameTypeArgs);
    TEST_RUN(CreateInstanceFromLambda, Basic_MultiDiffTypeArgs);

    TEST_RUN(CreateInstanceFromLambda, Capture_Value);
    TEST_RUN(CreateInstanceFromLambda, Capture_Reference);
    TEST_RUN(CreateInstanceFromLambda, make_function_NonArg);
    TEST_RUN(CreateInstanceFromLambda, make_function_SingleArg);

    TEST_RUN(CreateInstanceFromLambda, Mutable_Lambda);
    TEST_RUN(CreateInstanceFromLambda, Noexcept_Lambda);
    TEST_RUN(CreateInstanceFromLambda, Return_Void);
    TEST_RUN(CreateInstanceFromLambda, Constexpr_Lambda);
    TEST_RUN(CreateInstanceFromLambda, Nested_Lambda);
}

// Basic Non Arg
TEST(CreateInstanceFromLambda, Basic_NonArg) {
    constexpr int expect_ret = 1343249;
    embed::function<int()> fn = []() -> int { return expect_ret; };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(), expect_ret, "%d");

    return 0;
}

// Basic Single Arg
TEST(CreateInstanceFromLambda, Basic_SingleArg) {
    constexpr float expect_ret = 3.141592f;
    embed::function<float(float)> fn = [](float f) -> float { return f; };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(expect_ret), expect_ret, "%f");

    return 0;
}

// Multi Same Type Args (Lambda)
TEST(CreateInstanceFromLambda, Basic_MultiSameTypeArgs) {
    constexpr int expect_sum = 100;
    embed::function<int(int, int, int)> fn = [](int a, int b, int c) -> int { 
        return a + b + c; 
    };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(10, 20, 70), expect_sum, "%d");
    
    // Boundary value test
    ASSERT_EQ(fn(INT_MIN, 1, INT_MAX), 0, "%d");
    return 0;
}

// Multi Different Type Args (Lambda)
TEST(CreateInstanceFromLambda, Basic_MultiDiffTypeArgs) {
    embed::function<double(char, long, float)> fn = [](char a, long b, float c) -> double { 
        return static_cast<double>(a) + b + c; 
    };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ_F(fn('A', 1000000L, 3.14159f), 65 + 1000000 + 3.14159, 1e-6f);
    return 0;
}

// make_function Non Arg
TEST(CreateInstanceFromLambda, make_function_NonArg) {
    constexpr int expect_ret = 1343249;
    auto fn = embed::make_function([]() -> int { return expect_ret; });

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(), expect_ret, "%d");

    return 0;
}

// make_function Single Arg
TEST(CreateInstanceFromLambda, make_function_SingleArg) {
    constexpr float expect_ret = 3.141592f;
    auto fn = embed::make_function([](float f) -> float { return f; });

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(expect_ret), expect_ret, "%f");

    return 0;
}

// Fn and C++17 deducing
TEST(CreateInstanceFromLambda, Fn_and_cpp17Deducing) {

    constexpr int expect_int_val = 230529;
    constexpr float expect_float_val = 3283.3285294f;

#if EMBED_CXX_VERSION < 201703L
    embed::Fn<int(), sizeof(void*)> fn1 = []() -> int { return expect_int_val; };
    embed::Fn<float(float), sizeof(void*)> fn2 = [](float f) -> float { return f; };
#else
    embed::Fn fn1 = []() -> int { return expect_int_val; };
    embed::Fn fn2 = [](float f) -> float { return f; };
#endif

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");

    ASSERT_EQ(fn1(), expect_int_val, "%d");
    ASSERT_EQ(fn2(expect_float_val), expect_float_val, "%f");

    return 0;
}

// Value Capture Lambda
TEST(CreateInstanceFromLambda, Capture_Value) {
    int capture_val = 9527;
    embed::function<int()> fn = [capture_val]() -> int { 
        return capture_val; 
    };

    capture_val = 0; // Verify capture is by value (no change)
    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(), 9527, "%d");
    return 0;
}

// Reference Capture Lambda
TEST(CreateInstanceFromLambda, Capture_Reference) {
    int capture_val = 9527;
    embed::function<int()> fn = [&capture_val]() -> int { 
        return capture_val; 
    };

    capture_val = 10086; // Verify capture is by reference (value changes)
    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(), 10086, "%d");
    return 0;
}

// Mutable Lambda (modify captured value)
TEST(CreateInstanceFromLambda, Mutable_Lambda) {
    int capture_val = 0;
    embed::function<void()> fn = [capture_val]() mutable -> void { 
        capture_val = 12345; 
    };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    fn();
    ASSERT_EQ(capture_val, 0, "%d"); // Original value unchanged (value capture)
    return 0;
}

// Noexcept Lambda
TEST(CreateInstanceFromLambda, Noexcept_Lambda) {
    embed::function<int(int)> fn = [](int a) noexcept -> int { 
        return a * 2; 
    };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(50), 100, "%d");
    return 0;
}

// Return Void Lambda
TEST(CreateInstanceFromLambda, Return_Void) {
    int flag = 0;
    embed::function<void()> fn = [&flag]() -> void { 
        flag = 1; 
    };

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    fn();
    ASSERT_EQ(flag, 1, "%d");
    return 0;
}

// Constexpr Lambda (C++20+)
TEST(CreateInstanceFromLambda, Constexpr_Lambda) {
#if EMBED_CXX_VERSION >= 202002L 
    constexpr auto lambda = [](int a) constexpr -> int { return a * 3; };
    embed::function<int(int)> fn = lambda;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    constexpr int test_val = 7;
    ASSERT_EQ(fn(test_val), 21, "%d");
#endif

    return 0;
}


// Nested Lambda
TEST(CreateInstanceFromLambda, Nested_Lambda) {
    embed::function<int(int)> outer_fn = [](int a) -> int {
        // Nested lambda as return value
        auto inner_fn = [](int b) -> int { return b * b; };
        return inner_fn(a);
    };

    ASSERT_EQ(static_cast<bool>(outer_fn), true, "%d");
    ASSERT_EQ(outer_fn(10), 100, "%d");
    ASSERT_EQ(outer_fn(15), 225, "%d");
    return 0;
}
