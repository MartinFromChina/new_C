#ifndef __TEST_COMMON_H
#define __TEST_COMMON_H

#include "../../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../../google_test/include/include/gmock/gmock.h"


#ifdef __cplusplus
	extern "C"{
#endif


#define USE_INSERT_DEBUG 1


#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(method)  insert_##method
#else
	#define INSERT(method)  method
#endif


#ifdef __cplusplus
		}
#endif


#endif


