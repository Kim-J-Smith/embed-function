/**
 * Here is the test for constructors and `make_function` (free function).
 */
#include "embed/embed_function.hpp"
#include "test.hpp"

// Declare test functions
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Basic_SingleArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Basic_NonArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Basic_MultiSameTypeArgs);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Basic_MultiDiffTypeArgs);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, make_function_SingleArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, make_function_NonArg);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, make_function_MultiSameTypeArgs);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, make_function_MultiDiffTypeArgs);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Fn_and_cpp17Deducing);

TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Template_Function);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Function_Pointer);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Noexcept_FreeFunction);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Return_Void);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Overloaded_Function);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Static_Function);
TEST_FUNCTION_DECLARE(CreateInstanceFromFreeFunction, Namespaced_Function);

// Generate a main function for extern call.
TEST_SUBSYS(CreateInstanceFromFreeFunction, main) {
    TEST_RUN(CreateInstanceFromFreeFunction, Basic_SingleArg);
    TEST_RUN(CreateInstanceFromFreeFunction, Basic_NonArg);
    TEST_RUN(CreateInstanceFromFreeFunction, Basic_MultiSameTypeArgs);
    TEST_RUN(CreateInstanceFromFreeFunction, Basic_MultiDiffTypeArgs);
    TEST_RUN(CreateInstanceFromFreeFunction, make_function_SingleArg);
    TEST_RUN(CreateInstanceFromFreeFunction, make_function_NonArg);
    TEST_RUN(CreateInstanceFromFreeFunction, make_function_MultiSameTypeArgs);
    TEST_RUN(CreateInstanceFromFreeFunction, make_function_MultiDiffTypeArgs);
    TEST_RUN(CreateInstanceFromFreeFunction, Fn_and_cpp17Deducing);

    TEST_RUN(CreateInstanceFromFreeFunction, Template_Function);
    TEST_RUN(CreateInstanceFromFreeFunction, Function_Pointer);
    TEST_RUN(CreateInstanceFromFreeFunction, Noexcept_FreeFunction);
    TEST_RUN(CreateInstanceFromFreeFunction, Return_Void);
    TEST_RUN(CreateInstanceFromFreeFunction, Overloaded_Function);
    TEST_RUN(CreateInstanceFromFreeFunction, Static_Function);
    TEST_RUN(CreateInstanceFromFreeFunction, Namespaced_Function);
}


/// TEST: Create an embed::function instance from free function.
int testUse__free_function_SingleArg(int a) { return a; }
int testUse__free_function_NonArg() { return INT_MAX; }
int testUse__free_function_MultiSameTypeArgs(int a, int b, int c) { return a+b+c; }
float testUse__free_function_MultiDiffTypeArgs(char a, int b, float c) { return a+b+c; }

// Basic embed::function constructor. (free function - Single Arg)
TEST(CreateInstanceFromFreeFunction, Basic_SingleArg) {
    embed::function<int(int)> fn = testUse__free_function_SingleArg;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");

    for (int i = -65535 - 100; i < 65535 + 100; i++)
        ASSERT_EQ(fn(i), testUse__free_function_SingleArg(i), "%d");

    return 0;
}

// Basic embed::function constructor. (free function - Non Arg)
TEST(CreateInstanceFromFreeFunction, Basic_NonArg) {
    embed::function<int()> fn = testUse__free_function_NonArg;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(), INT_MAX, "%d");

    return 0;
}

// Basic embed::function constructor. (free function - Multi Same Type Arg)
TEST(CreateInstanceFromFreeFunction, Basic_MultiSameTypeArgs) {
    embed::function<int(int,int,int)> fn = testUse__free_function_MultiSameTypeArgs;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");

    constexpr int N = 257;
    for (int i = (~N) + 1; i <= N; i++)
        for (int j = (~N) + 1; j <= N; j++) {
            ASSERT_EQ(
                fn(i, j, 1), 
                testUse__free_function_MultiSameTypeArgs(i, j, 1),
                "%d"
            );
        }
    return 0;
}

// Basic embed::function constructor. (free function - Multi Diff Type Arg)
TEST(CreateInstanceFromFreeFunction, Basic_MultiDiffTypeArgs) {
    embed::function<float(char,int,float)> fn = testUse__free_function_MultiDiffTypeArgs;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");

    for (char i = 0; i < 126; i++)
        for (int j = -257; j < 257; j++)
            for (float k = -3.1415926f; k < 100; k += 31.415926535f)
                ASSERT_EQ(
                    fn(i, j, k), testUse__free_function_MultiDiffTypeArgs(i, j, k),
                    "%f"
                );

    return 0;
}

// Use make_function to create the instance. (free function - Single Arg)
TEST(CreateInstanceFromFreeFunction, make_function_SingleArg) {
    auto fn = embed::make_function(testUse__free_function_SingleArg);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");

    for (int i = -65535 - 100; i < 65535 + 100; i++)
        ASSERT_EQ(fn(i), testUse__free_function_SingleArg(i), "%d");

    return 0;
}

// Use make_function to create the instance. (free function - Non Arg)
TEST(CreateInstanceFromFreeFunction, make_function_NonArg) {
    auto fn = embed::make_function(testUse__free_function_NonArg);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");
    ASSERT_EQ(fn(), INT_MAX, "%d");

    return 0;
}

// Use make_function to create the instance. (free function - Multi Same Type Arg)
TEST(CreateInstanceFromFreeFunction, make_function_MultiSameTypeArgs) {
    auto fn = embed::make_function(testUse__free_function_MultiSameTypeArgs);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");

    constexpr int N = 257;
    for (int i = (~N) + 1; i <= N; i++)
        for (int j = (~N) + 1; j <= N; j++) {
            ASSERT_EQ(
                fn(i, j, 1), 
                testUse__free_function_MultiSameTypeArgs(i, j, 1),
                "%d"
            );
        }
    return 0;
}

// Use make_function to create the instance. (free function - Multi Diff Type Arg)
TEST(CreateInstanceFromFreeFunction, make_function_MultiDiffTypeArgs) {
    auto fn = embed::make_function(testUse__free_function_MultiDiffTypeArgs);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn.is_empty(), false, "%d");

    for (char i = 0; i < 126; i++)
        for (int j = -257; j < 257; j++)
            for (float k = -3.1415926f; k < 100; k += 31.415926535f)
                ASSERT_EQ(
                    fn(i, j, k), testUse__free_function_MultiDiffTypeArgs(i, j, k),
                    "%f"
                );

    return 0;
}

// Create instance with the basic type embed::Fn. (free function)
// And Try deducing the template type. (C++17)
TEST(CreateInstanceFromFreeFunction, Fn_and_cpp17Deducing) {

#if EMBED_CXX_VERSION < 201703L
    embed::Fn<int(int), sizeof(void*)> fn1 = testUse__free_function_SingleArg;
    embed::Fn<int(), sizeof(void*)> fn2 = testUse__free_function_NonArg;
    embed::Fn<int(int,int,int), sizeof(void*)> fn3 = testUse__free_function_MultiSameTypeArgs;
    embed::Fn<float(char,int,float), sizeof(void*)> fn4 = testUse__free_function_MultiDiffTypeArgs;
#else
    embed::Fn fn1 = testUse__free_function_SingleArg;
    embed::Fn fn2 = testUse__free_function_NonArg;
    embed::Fn fn3 = testUse__free_function_MultiSameTypeArgs;
    embed::Fn fn4 = testUse__free_function_MultiDiffTypeArgs;
#endif

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn3), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn4), true, "%d");

    ASSERT_EQ(fn1(12344321), testUse__free_function_SingleArg(12344321), "%d");
    ASSERT_EQ(fn2(), testUse__free_function_NonArg(), "%d");
    ASSERT_EQ(fn3(3141,1353,4624), testUse__free_function_MultiSameTypeArgs(3141,1353,4624), "%d");
    ASSERT_EQ(fn4('Z', 352653, 2141.315f), testUse__free_function_MultiDiffTypeArgs('Z', 352653, 2141.315f), "%f");

    return 0;
}

template <typename T>
bool testUse__free_function_Template_greater(T a, T b) {
    return a > b;
}

template <typename T, T t>
T testUse__free_function_Template_add(T t_) {
    return t + t_;
}

// template function test
TEST(CreateInstanceFromFreeFunction, Template_Function) {

    embed::function<bool(int,int)> fn1 = testUse__free_function_Template_greater<int>;
    embed::function<bool(float,float)> fn2 = testUse__free_function_Template_greater<float>;

    auto fn3 = embed::make_function(testUse__free_function_Template_greater<char>);
    auto fn4 = embed::make_function(testUse__free_function_Template_add<int, 3>);

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn3), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn4), true, "%d");

    ASSERT_EQ(fn1(1145, 1144), true, "%d");
    ASSERT_EQ(fn2(3.140f, 3.1415f), false, "%d");
    ASSERT_EQ(fn3('A', 'B'), false, "%d");
    ASSERT_EQ(fn4(7), 10, "%d");

    return 0;
}


// Helper free functions for extended tests
int testUse__free_function_Pointer(int a) { return a * 5; }
void testUse__free_function_Void(int& val) { val = 9999; }
int testUse__free_function_Noexcept(int a) noexcept { return a + 100; }

// Overloaded functions
int testUse__free_function_Overload(int a) { return a; }
float testUse__free_function_Overload(float a) { return a * 2; }

// Static function
namespace TestNS {
static int static_free_func(int a) { return a * 10; }

// Namespaced function
int namespaced_free_func(uint64_t a) { return static_cast<int>(a % 1000); }
} // namespace TestNS

// Recursive function
int testUse__free_function_Recursive(int n) {
    if (n <= 1) return 1;
    return n * testUse__free_function_Recursive(n - 1);
}

// Function Pointer test
TEST(CreateInstanceFromFreeFunction, Function_Pointer) {
    int (*fp)(int) = testUse__free_function_Pointer;
    embed::function<int(int)> fn = fp;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(20), 100, "%d");
    return 0;
}

// Noexcept Free Function test
TEST(CreateInstanceFromFreeFunction, Noexcept_FreeFunction) {
    embed::function<int(int)> fn = testUse__free_function_Noexcept;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(50), 150, "%d");
    return 0;
}

// Return Void Free Function test
TEST(CreateInstanceFromFreeFunction, Return_Void) {
    int val = 0;
    embed::function<void(int&)> fn = testUse__free_function_Void;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    fn(val);
    ASSERT_EQ(val, 9999, "%d");
    return 0;
}

// Overloaded Free Function test (explicit type cast)
TEST(CreateInstanceFromFreeFunction, Overloaded_Function) {
    // Explicitly cast to resolve overload
    embed::function<int(int)> fn_int = static_cast<int(*)(int)>(testUse__free_function_Overload);
    embed::function<float(float)> fn_float = static_cast<float(*)(float)>(testUse__free_function_Overload);

    auto fn1 = embed::make_function(static_cast<int(&)(int)>(testUse__free_function_Overload));
    auto fn2 = embed::make_function(static_cast<float(&)(float)>(testUse__free_function_Overload));

    ASSERT_EQ(static_cast<bool>(fn_int), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn_float), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");

    ASSERT_EQ(fn_int(10), 10, "%d");
    ASSERT_EQ_F(fn_float(3.14f), 6.28f, 1e-6f);
    ASSERT_EQ(fn1(11), 11, "%d");
    ASSERT_EQ_F(fn2(3.15f), 6.30f, 1e-6f);
    return 0;
}

// Static Free Function test
TEST(CreateInstanceFromFreeFunction, Static_Function) {
    embed::function<int(int)> fn = TestNS::static_free_func;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(5), 50, "%d");
    ASSERT_EQ(fn(7), 70, "%d");
    return 0;
}

// Namespaced Free Function test
TEST(CreateInstanceFromFreeFunction, Namespaced_Function) {
    embed::function<int(uint64_t)> fn = TestNS::namespaced_free_func;

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(123456789ULL), 789, "%d");
    return 0;
}
