#include <iostream>
/*
#include "../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest.h"
#include "../../tdd/google/gmock-1.7.0/include/gmock/gmock.h"
*/
#include "../../tdd/google/gmock-1.7.0/gtest/src/gtest-all.cc"
#include "../../tdd/google/gmock-1.7.0/src/gmock-all.cc"

bool test_fun(void)
{
  return true;
}

TEST(IsPrimeTest, Positive) {
  EXPECT_TRUE(test_fun);
  EXPECT_FALSE(test_fun);
}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from tdd_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
