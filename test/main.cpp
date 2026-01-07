#include "test.hpp"

TEST_SUBSYS_DECLARE(CreateInstanceFromFreeFunction, main);
TEST_SUBSYS_DECLARE(CreateInstanceFromLambda, main);
TEST_SUBSYS_DECLARE(CreateInstanceFromMemberFunction, main);
TEST_SUBSYS_DECLARE(CreateInstanceFromFunctor, main);
TEST_SUBSYS_DECLARE(AssignTest, main);

int main()
{

    TEST_RUN_SUBSYS(CreateInstanceFromFreeFunction, main);
    TEST_RUN_SUBSYS(CreateInstanceFromLambda, main);
    TEST_RUN_SUBSYS(CreateInstanceFromMemberFunction, main);
    TEST_RUN_SUBSYS(CreateInstanceFromFunctor, main);
    TEST_RUN_SUBSYS(AssignTest, main);

    return 0;
}
