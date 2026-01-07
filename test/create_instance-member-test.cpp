/**
 * Test for constructors and `make_function` (member function).
 */
#include "embed/embed_function.hpp"
#include "test.hpp"
#include <string>

// Declare member function test functions
TEST_FUNCTION_DECLARE(CreateInstanceFromMemberFunction, NonStatic_MemberFunc);
TEST_FUNCTION_DECLARE(CreateInstanceFromMemberFunction, Static_MemberFunc);
TEST_FUNCTION_DECLARE(CreateInstanceFromMemberFunction, Const_MemberFunc);
TEST_FUNCTION_DECLARE(CreateInstanceFromMemberFunction, Volatile_MemberFunc);
TEST_FUNCTION_DECLARE(CreateInstanceFromMemberFunction, MemberFunc_WithArgs);

// Test subsys main entry
TEST_SUBSYS(CreateInstanceFromMemberFunction, main) {
    TEST_RUN(CreateInstanceFromMemberFunction, NonStatic_MemberFunc);
    TEST_RUN(CreateInstanceFromMemberFunction, Static_MemberFunc);
    TEST_RUN(CreateInstanceFromMemberFunction, Const_MemberFunc);
    TEST_RUN(CreateInstanceFromMemberFunction, Volatile_MemberFunc);
    TEST_RUN(CreateInstanceFromMemberFunction, MemberFunc_WithArgs);
}

// Test class for member function tests
class TestClass {
public:
    int non_static_func() { return 1234; }
    static int static_func() { return 5678; }
    int const_func() const { return 9012; }
    int volatile_func() volatile { return 3456; }
    std::string arg_func(const std::string& str, int num) {
        return str + std::to_string(num);
    }
};

// Non-static Member Function test
TEST(CreateInstanceFromMemberFunction, NonStatic_MemberFunc) {
    TestClass obj;
    // Bind object to non-static member function
    auto fn = embed::make_function(&TestClass::non_static_func);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(obj), 1234, "%d");
    return 0;
}

// Static Member Function test
TEST(CreateInstanceFromMemberFunction, Static_MemberFunc) {
    auto fn = embed::make_function(&TestClass::static_func);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(), 5678, "%d");
    return 0;
}

// Const Member Function test
TEST(CreateInstanceFromMemberFunction, Const_MemberFunc) {
    const TestClass obj;
    auto fn = embed::make_function(&TestClass::const_func);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(obj), 9012, "%d");
    return 0;
}

// Volatile Member Function test
TEST(CreateInstanceFromMemberFunction, Volatile_MemberFunc) {
    volatile TestClass obj;
    auto fn = embed::make_function(&TestClass::volatile_func);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ(fn(obj), 3456, "%d");
    return 0;
}

// Member Function with Args test
TEST(CreateInstanceFromMemberFunction, MemberFunc_WithArgs) {
    TestClass obj;
    auto fn = embed::make_function(&TestClass::arg_func);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    ASSERT_EQ_STR(fn(obj, "test_", 9527).c_str(), "test_9527");
    return 0;
}
