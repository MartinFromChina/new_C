#include <iostream>
using namespace std;
#include "./google_test/include/gtest/include/gtest/gtest.h"
#include "./google_test/include/include/gmock/gmock.h"


bool test_fun(void)
{
  return true;
}

TEST(IsPrimeTest, Positive) {
  EXPECT_TRUE(test_fun);
  //EXPECT_FALSE(test_fun);
}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from tdd_main.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
