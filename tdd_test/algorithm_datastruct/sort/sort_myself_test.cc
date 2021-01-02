#include "tdd_common.h"
#include "sort_myself.h"

TEST(sort_test,from_small_to_big)
{
	uint16_t origin[10] = {3,6,4,9,1,5,3,7,9,11};
	uint16_t dest[10] = {0,0,0,0,0,0,0,0,0,0};
	
	sort_from_small_to_big(origin,dest,sizeof(origin)/sizeof(uint16_t));
	EXPECT_EQ(1,dest[0]);
	EXPECT_EQ(3,dest[1]);
	EXPECT_EQ(3,dest[2]);
	EXPECT_EQ(4,dest[3]);
	EXPECT_EQ(5,dest[4]);
	EXPECT_EQ(6,dest[5]);
	EXPECT_EQ(7,dest[6]);
	EXPECT_EQ(9,dest[7]);
	EXPECT_EQ(9,dest[8]);
	EXPECT_EQ(11,dest[9]);
}
