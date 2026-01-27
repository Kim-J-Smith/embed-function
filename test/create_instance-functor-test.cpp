/**
 * Test for constructors and `make_function` (functor).
 */
#include "embed/embed_function.hpp"
#include "test.hpp"

TEST_FUNCTION_DECLARE(CreateInstanceFromFunctor, Copyable);
TEST_FUNCTION_DECLARE(CreateInstanceFromFunctor, NonCopyable);
TEST_FUNCTION_DECLARE(CreateInstanceFromFunctor, Qualifier_Single);

TEST_SUBSYS(CreateInstanceFromFunctor, main) {
    TEST_RUN(CreateInstanceFromFunctor, Copyable);
    TEST_RUN(CreateInstanceFromFunctor, NonCopyable);
    TEST_RUN(CreateInstanceFromFunctor, Qualifier_Single);
}

class testUse__CopyableObject
{
public:
    int operator()(int a) { return a; }
};

class testUse__NonCopyableObject
{
public:
    int operator()(int a) { return a; }

    testUse__NonCopyableObject() = default;
    testUse__NonCopyableObject(const testUse__NonCopyableObject&) = delete;
    testUse__NonCopyableObject(testUse__NonCopyableObject&&) = default;
};

TEST(CreateInstanceFromFunctor, Copyable) {
    embed::function<int(int)> fn1 = testUse__CopyableObject();
    embed::function<int(int)> fn2 = testUse__CopyableObject{};
    auto fn3 = embed::make_function(testUse__CopyableObject());
    auto fn4 = embed::make_function(testUse__CopyableObject{});

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn3), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn4), true, "%d");

    return 0;
}

TEST(CreateInstanceFromFunctor, NonCopyable) {
#if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    embed::function<int(int)> fn1 = testUse__NonCopyableObject();
    embed::function<int(int)> fn2 = testUse__NonCopyableObject{};
    auto fn3 = embed::make_function(testUse__NonCopyableObject());
    auto fn4 = embed::make_function(testUse__NonCopyableObject{});

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn3), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn4), true, "%d");

    fn1 = nullptr;
    fn2 = nullptr;
    fn3 = nullptr;
    ASSERT_EQ(static_cast<bool>(fn1), false, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), false, "%d");
    ASSERT_EQ(static_cast<bool>(fn3), false, "%d");

    fn1 = std::move(fn4);
    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn4), false, "%d");

    std::swap(fn1, fn3);
    ASSERT_EQ(static_cast<bool>(fn1), false, "%d");
    ASSERT_EQ(static_cast<bool>(fn3), true, "%d");
#endif
    return 0;
}

struct testUse__Qualifier_C {
    int operator()(int) const { return 1234; }
};
struct testUse__Qualifier_V {
    int operator()(int) volatile { return 2345; }
};
struct testUse__Qualifier_LRef {
    int operator()(int) & { return 3456; }
};
struct testUse__Qualifier_RRef {
    int operator()(int) && { return 4567; }
};

TEST(CreateInstanceFromFunctor, Qualifier_Single) {
    auto fn1 = embed::make_function(testUse__Qualifier_C{});
    auto fn2 = embed::make_function(testUse__Qualifier_V{});
    auto fn3 = embed::make_function(testUse__Qualifier_LRef{});
    auto fn4 = embed::make_function(testUse__Qualifier_RRef{});

    ASSERT_EQ(fn1(1), 1234, "%d");
    ASSERT_EQ(fn2(2), 2345, "%d");
    ASSERT_EQ(fn3(3), 3456, "%d");
    ASSERT_EQ(std::move(fn4)(4), 4567, "%d");

    return 0;
}

