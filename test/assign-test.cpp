#include "embed/embed_function.hpp"
#include "test.hpp"
#include <utility>

TEST_FUNCTION_DECLARE(AssignTest, Copy_Assignment);
TEST_FUNCTION_DECLARE(AssignTest, Move_Assignment);
TEST_FUNCTION_DECLARE(AssignTest, Nullptr_Assignment);

TEST_SUBSYS(AssignTest, main) {
    TEST_RUN(AssignTest, Copy_Assignment);
    TEST_RUN(AssignTest, Move_Assignment);
    TEST_RUN(AssignTest, Nullptr_Assignment);
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
    ASSERT_EQ(fn1(7), 14, "%d");
    ASSERT_EQ(fn2(7), 14, "%d");

    fn1 = test_assign_free_func2;
    ASSERT_EQ(fn1(7), 17, "%d");
    ASSERT_EQ(fn2(7), 14, "%d");

    return 0;
}

TEST(AssignTest, Move_Assignment) {
    embed::function<int(int)> fn1 = test_assign_free_func;
    embed::function<int(int)> fn2;

    fn2 = std::move(fn1);
    
    ASSERT_EQ(static_cast<bool>(fn1), false, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");
    ASSERT_EQ(fn2(6), 12, "%d");

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