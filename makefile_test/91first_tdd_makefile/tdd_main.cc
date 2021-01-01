#include <iostream>
#include "../../tdd/google/gmock-1.7.0/include/gmock/gmock.h"
#include "../../tdd/google/gmock-1.7.0/gtest/gtest.h"

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