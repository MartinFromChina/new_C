#ifndef __NODE_TEST_H
#define __NODE_TEST_H

#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"

#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#include "../node_interconnect.h"

/*

#if (SEGGER_RTT_DEBUG == 0)
	#ifdef DEBUG 
		#error [will take the risk that marco "DEBUG" cause hardfault in product !!!]
	#endif
#else 
	#ifndef DEBUG 
		#warning [will miss more detail when fatal error occur if disable marco "DEBUG"]
	#endif
#endif
*/

#endif
