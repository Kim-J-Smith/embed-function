#ifndef TEST_HPP___
#define TEST_HPP___

#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#if defined(EMBED_NO_STD_HEADER)
# include "embed/embed_function.hpp"
namespace std {
  using namespace embed::detail::fn_no_std;
}
#endif

////////////////////////////////////////////////////////////////

#define MESSAGE(msg) printf(__FILE__ "(" TEST_STR(__LINE__) "): " msg "\n")
#define MESSAGE_FMT(format, ...) printf(__FILE__ "(" TEST_STR(__LINE__) "): " format "\n", __VA_ARGS__)

#define MESSAGE_RUN(test_suite_name, test_name) \
  printf("[ RUN      ] " #test_suite_name " - " #test_name "\n")

#define MESSAGE_OK(test_suite_name, test_name) \
  printf("[       OK ] " #test_suite_name " - " #test_name "\n")

#define MESSAGE_FAIL(msg, ...) \
  do { printf("[  FAILED  ] "); MESSAGE_FMT(msg, __VA_ARGS__); } while(0)

#define MESSAGE_BEGIN(test_suite_name, test_name) \
  printf("[----------] [BEGIN] " #test_suite_name " - " #test_name "\n")

#define MESSAGE_END(test_suite_name, test_name) \
  do {printf("[----------] [END] " #test_suite_name " - " #test_name "\n\n"); fflush(stdout);} while(0)

////////////////////////////////////////////////////////////////

#define TEST_STR_(name, ...) #name
#define TEST_STR(name) TEST_STR_(name, )

#define TEST_CHECK_NAME_NOT_EMPTY(name) \
  static_assert(sizeof(TEST_STR(name)) > 0, "name must not be empty")

#define TEST_FUNCTION_NAME(test_suite_name, test_name)  \
  test_ ## test_suite_name ## _ ## test_name

#define TEST_FUNCTION_DECLARE(test_suite_name, test_name) \
  int TEST_FUNCTION_NAME(test_suite_name, test_name) ()

#define TEST(test_suite_name, test_name)    \
  TEST_CHECK_NAME_NOT_EMPTY(test_suite_name);\
  TEST_CHECK_NAME_NOT_EMPTY(test_name);\
  int TEST_FUNCTION_NAME(test_suite_name, test_name) ()

#define TEST_RUN(test_suite_name, test_name)  \
  do {\
    MESSAGE_RUN(test_suite_name, test_name);\
    int ret = TEST_FUNCTION_NAME(test_suite_name, test_name)();\
    if (ret == 0)\
      MESSAGE_OK(test_suite_name, test_name);\
  } while(0)

#define TEST_SUBSYS(test_suite_name, test_name)    \
  TEST_CHECK_NAME_NOT_EMPTY(test_suite_name);\
  TEST_CHECK_NAME_NOT_EMPTY(test_name);\
  void TEST_FUNCTION_NAME(test_suite_name, test_name) ()

#define TEST_SUBSYS_DECLARE(test_suite_name, test_name) \
  void TEST_FUNCTION_NAME(test_suite_name, test_name) ()

#define TEST_RUN_SUBSYS(test_suite_name, test_name)  \
  do {\
    MESSAGE_BEGIN(test_suite_name, test_name);\
    TEST_FUNCTION_NAME(test_suite_name, test_name)();\
    MESSAGE_END(test_suite_name, test_name);\
  } while(0)

#define ASSERT_EQ(A, B, type_format) \
  do {\
    if ((A) != (B)) {\
      MESSAGE_FAIL("Assert - Equal - Fail\n[---Cause--]: "\
        type_format " != " type_format, (A), (B));\
      return 1;\
    }\
  } while(0)

#define ASSERT_NE(A, B, type_format) \
  do {\
    if ((A) == (B)) {\
      MESSAGE_FAIL("Assert - Equal - Fail\n[---Cause--]: "\
        type_format " == " type_format, (A), (B));\
      return 2;\
    }\
  } while(0)

#define ASSERT_EQ_F(A, B, error_) \
  do {\
    if (((A) - (B) > error_) || ((A) - (B) < -error_)) {\
      MESSAGE_FAIL("Assert - Equal float - Fail\n[---Cause--]: "\
        "%f" " != " "%f", (A), (B));\
      return 3;\
    }\
  } while(0)

#define ASSERT_EQ_STR(A, B) \
  do {\
    if (strcmp(A, B)) {\
      MESSAGE_FAIL("Assert - Equal str - Fail\n[---Cause--]: "\
        "%s != %s" , (A), (B));\
      return 4;\
    }\
  } while(0)

#define ASSERT_FAIL(msg) \
  do {\
    MESSAGE_FAIL("%s", "Assert - Fail");\
    return 5;\
  } while(0)


class testUse__Base {
public:
  int member_var = 100;
};

class testUse__Base1 : virtual public testUse__Base {
public:
  virtual void foo() = 0;
  virtual int foo(int a) = 0;
  virtual float foo(float f) { return 3.14f + f; }
  virtual ~testUse__Base1() = default;
};

class testUse__Base2 : virtual public testUse__Base {
public:
  virtual void bar() const noexcept {};
  virtual int bar(int a) const noexcept { return a; }
  virtual float bar(float f) const noexcept { return 3.14f + f; }
  virtual ~testUse__Base2() = default;
};

#endif // TEST_HPP___

