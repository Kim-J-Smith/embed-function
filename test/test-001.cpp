// Test construction of `embed::function`
#include <cstdio>
#include <iostream>
#include <functional>
#include "embed/embed_function.hpp"

static void t1_001_normal_function(void) noexcept { printf("Here is normal function\n"); }

static int  t1_002_return_function(void) noexcept { printf("Here is return function\n");return 314; }

static void t1_003_one_arg_function(int a) noexcept {
    printf("Here is one arg function, a = %d\n", a);
}

static void t1_004_multi_args_function(int a, float b, double c, char d) noexcept {
    printf("Here is multi args function, a=%d, b=%f, c=%lf, d=%c\n", a,b,c,d);
}

struct t1_005_callable_struct {
    void operator()() noexcept { printf("Here is callable struct\n"); }

    int operator()(int a, float b, double c, char d) noexcept {
        printf("Here is callable struct, a=%d, b=%f, c=%lf, d=%c\n", a,b,c,d);
        return 31415;
    }

    float member_func(char a, int b) const noexcept {
        printf("Here is member function, a=%c, b=%d\n", a,b);
        return 3.1415926f;
    }

    float member_func_ref(char a, int b) & noexcept {
        printf("Here is member function, a=%c, b=%d\n", a,b);
        return 3.1415926f;
    }

    float member_func_const_ref(char a, int b) const & noexcept {
        printf("Here is member function, a=%c, b=%d\n", a,b);
        return 3.1415926f;
    }

};

struct t1_006_unique_callable {
    void operator()(int a) noexcept {
        printf("Here is unique callable struct, a=%d\n", a);
    }
};

void t1_007_override_func(int a) {
    printf("Here is override function, a=%d\n", a);
}

void t1_007_override_func(int a, float b) {
    printf("Here is override function, a=%d, b=%f\n", a,b);
}

struct t1_008_struct_02 {
    t1_008_struct_02(int a)
    {
        this->a = a;
    }
    int a{};
};

t1_008_struct_02 t1_009_return_a_class(int a) {
    printf("Here is return class function, a=%d\n", a);
    return t1_008_struct_02(a);
}

struct t1_010_struct_non_copyable
{
    t1_010_struct_non_copyable(const t1_010_struct_non_copyable&) = delete;

    t1_010_struct_non_copyable(t1_010_struct_non_copyable&& ) noexcept {}

    t1_010_struct_non_copyable() = default;

    void operator=(const t1_010_struct_non_copyable&) = delete;

    void operator()() noexcept { printf("Here is non-copyable struct\n"); }
};

#if ( EMBED_CXX_VERSION >= 202302L )

struct t1_011_struct_explicit_this
{
    void print__() const noexcept {
        printf("Here is explicit-this struct\n");
    }

    void operator()(this t1_011_struct_explicit_this& self) noexcept
    { self.print__(); }
};

#endif

void test_001()
{
    std::cout << "\n[START - test_001]\n" << std::endl;

    /// Strict and accurate type signatures

    embed::function<void()> fn1 = t1_001_normal_function;

    embed::function<int()> fn2 = t1_002_return_function;

    embed::function<void(int)> fn3 = t1_003_one_arg_function;

    embed::function<void(int,float,double,char)> fn4 = t1_004_multi_args_function;

    embed::function<void()> fn5 = t1_005_callable_struct();

    embed::function<void()> fn6 = t1_005_callable_struct{};

    embed::function<int(int,float,double,char)> fn7 = t1_005_callable_struct();

    embed::function<int(int,float,double,char)> fn8 = t1_005_callable_struct{};

    t1_005_callable_struct struct__001;
    embed::function<float(char,int)> fn9 = 
        [&struct__001](char a, int b) noexcept { return struct__001.member_func(a,b); };

#if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    embed::function<void()> fn10 = t1_010_struct_non_copyable{};
#endif

    std::cout << "<test_001>: [BEGIN] Strict and accurate type signatures" << std::endl;

    fn1();
    auto tmp_2 = fn2();
    fn3(1);
    fn4(1, 3.14f, 5.44, 'A');
    fn5();
    fn6();
    auto tmp_7 = fn7(1, 3.1415f, 2.7, 'B');
    auto tmp_8 = fn8(1, 3.1415f, 2.7, 'C');
    auto tmp_9 = fn9('D', 12345);
#if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    fn10();
#endif

    std::cout << "fn2() = " << tmp_2 << std::endl;
    std::cout << "fn7() = " << tmp_7 << std::endl;
    std::cout << "fn8() = " << tmp_8 << std::endl;
    std::cout << "fn9() = " << tmp_9 << std::endl;

    std::cout << "<test_001>: [END] Strict and accurate type signatures\n" << std::endl; // Strict and accurate type signatures end

    // Not strict type signature
#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable: 4244 4242 4305)
#endif

    embed::function<void()> fn20 = t1_002_return_function;

    embed::function<void(char,char,char,int)> fn21 = t1_004_multi_args_function;

    embed::function<void(float, double)> fn22 =
        [&struct__001](char a, int b) noexcept { return struct__001.member_func(a,b); };

    std::cout << "<test_001>: [BEGIN] Not strict type signature" << std::endl;

    fn20();
    fn21(31, 32, 33, 'A' | 0x1000);
    fn22(36.1415, 3.1415926);

    std::cout << "<test_001>: [END] Not strict type signature\n" << std::endl;

    // Test the `make_function`

    auto fn30 = embed::make_function<void()>(t1_001_normal_function);

    auto fn31 = embed::make_function<void()>(&t1_001_normal_function);

    auto fn32 = embed::make_function(t1_001_normal_function);

    auto fn33 = embed::make_function(t1_002_return_function);

    auto fn34 = embed::make_function(t1_004_multi_args_function);

    auto fn35 = embed::make_function<void()>(t1_005_callable_struct{});

    auto fn36 = embed::make_function<void(int,float,double,char)>(t1_005_callable_struct{});

    auto fn37 = embed::make_function<void(char, int)>(
        [&struct__001](char a, int b) noexcept { return struct__001.member_func(a,b); }
    );

    auto fn38 = embed::make_function(
        [&struct__001](char a, int b) noexcept { return struct__001.member_func(a,b); }
    );

    auto fn39 = embed::make_function(t1_006_unique_callable{});

    auto fn40 = embed::make_function(static_cast<void(*)(int)>(t1_007_override_func));

    auto fn41 = embed::make_function(static_cast<void(&)(int)>(t1_007_override_func));

    auto fn42 = embed::make_function(static_cast<void(*)(int,float)>(t1_007_override_func));

    auto fn43 = embed::make_function(static_cast<void(&)(int,float)>(t1_007_override_func));

    auto fn44 = embed::make_function(t1_009_return_a_class);

#if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    auto fn45 = embed::make_function(t1_010_struct_non_copyable{});
#endif

    auto fn46 = embed::make_function(&t1_005_callable_struct::member_func);

    auto fn47 = embed::make_function(&t1_005_callable_struct::member_func_ref);

    auto fn48 = embed::make_function(&t1_005_callable_struct::member_func_const_ref);

#if ( EMBED_CXX_VERSION >= 202302L )

    auto fn49 = embed::make_function(t1_011_struct_explicit_this{});

#endif

    std::cout << "fn46.bufsize = " << fn46.buffer_size << std::endl;

    std::cout << "<test_001>: [BEGIN] Test the `make_function`" << std::endl;

    fn30();
    fn31();
    fn32();
    auto tmp_33 = fn33();
    fn34(1, 3.14, 3.1415926535, 'E');
    fn35();
    fn36(1, 2.7, 2.70001, 'F');
    fn37('G', 4321);
    auto tmp_38 = fn38('H', 1234);
    fn39(313);
    fn40(40);
    fn41(41);
    fn42(42, 4200.1);
    fn43(43, 4300.1);
    fn44(44);
#if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    fn45();
#endif
    fn46(struct__001, 'H', 46);
    fn47(struct__001, 'I', 47);
    fn48(struct__001, 'J', 48);

#if ( EMBED_CXX_VERSION >= 202302L )
    fn49();
#endif

    std::cout << "fn33() = " << tmp_33 << std::endl;
    std::cout << "fn38() = " << tmp_38 << std::endl;

    std::cout << "<test_001>: [END] Test the `make_function`\n" << std::endl;


    // Test the template guide
#if EMBED_CXX_VERSION >= 201703L

    embed::Fn fn101 = t1_001_normal_function;

    embed::Fn fn102 = t1_002_return_function;

    embed::Fn fn103 = t1_003_one_arg_function;

    embed::Fn fn104 = t1_004_multi_args_function;

    embed::Fn fn105 = t1_006_unique_callable{};

    std::cout << "<test_001>: [BEGIN] Test the template guide\n" << std::endl;

    fn101();

    fn102();

    fn103(103);

    fn104(104, 1.04f, 1.004, 'L');

    fn105(105);

    std::cout << "<test_001>: [END] Test the template guide\n" << std::endl;

#endif // C++17

    std::cout << "[END - test_001] : ------ "  "OK"  " ------\n\n" << std::endl;
#if defined(_MSC_VER)
# pragma warning(pop)
#endif
}

