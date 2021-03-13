#ifndef __MOTIONFUSION_H
#define __MOTIONFUSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../../module/common/x_cross_platform.h"
#include "DataConvert.h"

X_Boolean MotionFusion(s_6D_data *p_6d,s_3_angle *p_angle);

#ifdef __cplusplus
	}
#endif

#endif
