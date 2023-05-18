#include <iostream>
#include "tdd_common.h"

#include <stdio.h>
//#define LOG_DEBUG_METHOD   printf
//#include "../../../common/InsertLog/InsertLogDebug.h"

using namespace std;

TEST(nothing,ok)
{

}
/*************************************************************************************/

/*************************************************************************************/

/*************************************************************************************/

/*************************************************************************************/
GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  #if (WINDOWS_64_SYSTEM == 0)
  	return
  #else
	int value = 
  #endif
  RUN_ALL_TESTS();

#if (WINDOWS_64_SYSTEM!=0)
  value = value;
  while(1)
  {

  }
#endif
}
