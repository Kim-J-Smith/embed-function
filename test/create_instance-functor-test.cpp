/**
 * Test for constructors and `make_function` (functor).
 */
#include "embed/embed_function.hpp"
#include "test.hpp"

TEST_FUNCTION_DECLARE(CreateInstanceFromFunctor, Copyable);
TEST_FUNCTION_DECLARE(CreateInstanceFromFunctor, NonCopyable);

TEST_SUBSYS(CreateInstanceFromFunctor, main) {
    TEST_RUN(CreateInstanceFromFunctor, Copyable);
    TEST_RUN(CreateInstanceFromFunctor, NonCopyable);
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
