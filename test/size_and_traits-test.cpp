#include "embed/embed_function.hpp"
#include "test.hpp"

TEST_FUNCTION_DECLARE(SizeAndTraitsTest, LayoutMatch);
TEST_FUNCTION_DECLARE(SizeAndTraitsTest, NoThrowDefaultConstructibleTest);
TEST_FUNCTION_DECLARE(SizeAndTraitsTest, NoThrowCopyConstructibleTest);
TEST_FUNCTION_DECLARE(SizeAndTraitsTest, NoThrowMoveConstructibleTest);
TEST_FUNCTION_DECLARE(SizeAndTraitsTest, NoThrowSwap);

TEST_SUBSYS(SizeAndTraitsTest, main) {
    TEST_RUN(SizeAndTraitsTest, LayoutMatch);
    TEST_RUN(SizeAndTraitsTest, NoThrowDefaultConstructibleTest);
    TEST_RUN(SizeAndTraitsTest, NoThrowCopyConstructibleTest);
    TEST_RUN(SizeAndTraitsTest, NoThrowMoveConstructibleTest);
    TEST_RUN(SizeAndTraitsTest, NoThrowSwap);
}

TEST(SizeAndTraitsTest, LayoutMatch) {
    using fn_t = embed::function<void()>;

    constexpr std::size_t expect_size = 
        2 * sizeof(void*) + sizeof(void*) * fn_t::is_fast_mode;

    fn_t fn;

    ASSERT_EQ(expect_size, sizeof(fn), "%zu");

    return 0;
}

TEST(SizeAndTraitsTest, NoThrowDefaultConstructibleTest) {
    using fn1_t = embed::function<void()>;
    using fn2_t = embed::function<void(int)>;
    using fn3_t = embed::function<void(int, double)>;
    using fn4_t = embed::function<char(int, double)>;
#if !defined(EMBED_NO_STD_HEADER)
    ASSERT_EQ(std::is_nothrow_default_constructible<fn1_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_default_constructible<fn2_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_default_constructible<fn3_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_default_constructible<fn4_t>::value, true, "%d");
#endif
    return 0;
}

TEST(SizeAndTraitsTest, NoThrowCopyConstructibleTest) {
    using fn1_t = embed::function<void()>;
    using fn2_t = embed::function<void(int)>;
    using fn3_t = embed::function<void(int, double)>;
    using fn4_t = embed::function<char(int, double)>;
#if !defined(EMBED_NO_STD_HEADER)
    ASSERT_EQ(std::is_nothrow_copy_constructible<fn1_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_copy_constructible<fn2_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_copy_constructible<fn3_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_copy_constructible<fn4_t>::value, true, "%d");
#endif
    return 0;
}

TEST(SizeAndTraitsTest, NoThrowMoveConstructibleTest) {
    using fn1_t = embed::function<void()>;
    using fn2_t = embed::function<void(int)>;
    using fn3_t = embed::function<void(int, double)>;
    using fn4_t = embed::function<char(int, double)>;
#if !defined(EMBED_NO_STD_HEADER)
    ASSERT_EQ(std::is_nothrow_move_constructible<fn1_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_move_constructible<fn2_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_move_constructible<fn3_t>::value, true, "%d");
    ASSERT_EQ(std::is_nothrow_move_constructible<fn4_t>::value, true, "%d");
#endif
    return 0;
}

TEST(SizeAndTraitsTest, NoThrowSwap) {
    using fn1_t = embed::function<void()>;
    using fn2_t = embed::function<void(int)>;
    using fn3_t = embed::function<void(int, double)>;
    using fn4_t = embed::function<char(int, double)>;

    fn1_t fn1_1, fn1_2;
    fn2_t fn2_1, fn2_2;
    fn3_t fn3_1, fn3_2;
    fn4_t fn4_1, fn4_2;

    ASSERT_EQ(noexcept(std::swap(fn1_1, fn1_2)), true, "%d");
    ASSERT_EQ(noexcept(std::swap(fn2_1, fn2_2)), true, "%d");
    ASSERT_EQ(noexcept(std::swap(fn3_1, fn3_2)), true, "%d");
    ASSERT_EQ(noexcept(std::swap(fn4_1, fn4_2)), true, "%d");

    return 0;
}
