#include <iostream>
#include "tdd_common.h"

using namespace std;

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from tdd_main.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
