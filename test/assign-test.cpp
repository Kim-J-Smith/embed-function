#include "embed/embed_function.hpp"
#include "test.hpp"

TEST_FUNCTION_DECLARE(AssignTest, Copy_Assignment);
TEST_FUNCTION_DECLARE(AssignTest, Move_Assignment);
TEST_FUNCTION_DECLARE(AssignTest, Nullptr_Assignment);
TEST_FUNCTION_DECLARE(AssignTest, Similar_Assignment);

TEST_SUBSYS(AssignTest, main) {
    TEST_RUN(AssignTest, Copy_Assignment);
    TEST_RUN(AssignTest, Move_Assignment);
    TEST_RUN(AssignTest, Nullptr_Assignment);
    TEST_RUN(AssignTest, Similar_Assignment);
}

int test_assign_free_func(int a) { return a * 2; }
int test_assign_free_func2(int a) { return a + 10; }
void test_assign_free_func_void() {}

TEST(AssignTest, Copy_Assignment) {
    embed::function<int(int)> fn1 = test_assign_free_func;
    embed::function<int(int)> fn2;

    fn2 = fn1;
    
    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");

    std::swap(fn1, fn2);

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");

    ASSERT_EQ(fn1(7), 14, "%d");
    ASSERT_EQ(fn2(7), 14, "%d");

    fn1 = test_assign_free_func2;

    std::swap(fn1, fn2);
    ASSERT_EQ(fn1(7), 14, "%d");
    ASSERT_EQ(fn2(7), 17, "%d");

    return 0;
}

struct testUse__MoveOnlyClass_
{
    int operator()(int a) const { return 2*a; }
    testUse__MoveOnlyClass_() = default;
    testUse__MoveOnlyClass_(const testUse__MoveOnlyClass_&) = delete;
    testUse__MoveOnlyClass_(testUse__MoveOnlyClass_&&) = default;
};

TEST(AssignTest, Move_Assignment) {
#if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    embed::function<int(int)> fn1 = test_assign_free_func;
    embed::function<int(int)> fn2;

    fn2 = std::move(fn1);

    fn2 = testUse__MoveOnlyClass_{};
    
    ASSERT_EQ(static_cast<bool>(fn1), false, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");
    ASSERT_EQ(fn2(6), 12, "%d");

    fn1 = std::move(fn2);
    ASSERT_EQ(static_cast<bool>(fn2), false, "%d");
    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(fn1(6), 12, "%d");
#endif
    return 0;
}

TEST(AssignTest, Nullptr_Assignment) {
    embed::function<int(int)> fn = test_assign_free_func;
    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    
    fn = nullptr;
    ASSERT_EQ(static_cast<bool>(fn), false, "%d");
    ASSERT_EQ(fn.is_empty(), true, "%d");

    embed::function<void()> fn_void = test_assign_free_func_void;
    ASSERT_EQ(static_cast<bool>(fn_void), true, "%d");
    
    fn_void = nullptr;
    ASSERT_EQ(static_cast<bool>(fn_void), false, "%d");

    return 0;
}

TEST(AssignTest, Similar_Assignment) {
    using fn_similar_t = embed::function<void(), sizeof(void*)>;
    using fn_t = embed::function<void(), fn_similar_t::buffer_size + 1>;

    ASSERT_NE(fn_t::buffer_size, fn_similar_t::buffer_size, "%zu");

#if !defined(EMBED_NO_STD_HEADER)
    ASSERT_EQ((std::is_constructible<fn_t, fn_similar_t>::value), true, "%d");
    ASSERT_EQ((std::is_assignable<fn_t, fn_similar_t>::value), true, "%d");
#endif

    return 0;
}
