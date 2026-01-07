#include "test.hpp"

TEST_SUBSYS_DECLARE(CreateInstanceFromFreeFunction, main);
TEST_SUBSYS_DECLARE(CreateInstanceFromLambda, main);
TEST_SUBSYS_DECLARE(CreateInstanceFromMemberFunction, main);

int main()
{

    TEST_RUN_SUBSYS(CreateInstanceFromFreeFunction, main);
    TEST_RUN_SUBSYS(CreateInstanceFromLambda, main);
    TEST_RUN_SUBSYS(CreateInstanceFromMemberFunction, main);

    return 0;
}
