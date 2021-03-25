#include "MotionFusion.h"

X_Boolean MotionFusion(s_6D_data *p_6d,s_3_angle *p_angle)
{
	p_6d = p_6d;p_angle = p_angle;
	//to do CalibrationProcess; empty for now 

	return X_False;
}

/*
static X_Void MotionFusionQuaternionApply(Quaternion const* pQuaternion, Vector3DFixed const* pVector, Vector3DFixed* pResult)
{
    FixedPointNumber sx, sy, sz, xy, yz, zx, xx, yy, zz;

    sx = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->scalar * pQuaternion->vector_x / (FIXED_POINT_NUMBER_SCALE / 2));
    sy = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->scalar * pQuaternion->vector_y / (FIXED_POINT_NUMBER_SCALE / 2));
    sz = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->scalar * pQuaternion->vector_z / (FIXED_POINT_NUMBER_SCALE / 2));
    xy = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->vector_x * pQuaternion->vector_y / (FIXED_POINT_NUMBER_SCALE / 2));
    yz = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->vector_y * pQuaternion->vector_z / (FIXED_POINT_NUMBER_SCALE / 2));
    zx = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->vector_z * pQuaternion->vector_x / (FIXED_POINT_NUMBER_SCALE / 2));
    xx = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->vector_x * pQuaternion->vector_x / (FIXED_POINT_NUMBER_SCALE / 2));
    yy = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->vector_y * pQuaternion->vector_y / (FIXED_POINT_NUMBER_SCALE / 2));
    zz = (FixedPointNumber)((FixedPointNumberProduct)pQuaternion->vector_z * pQuaternion->vector_z / (FIXED_POINT_NUMBER_SCALE / 2));

    pResult->x = ((FixedPointNumberProduct)(ConvertToFixedPointNumber(1) - yy - zz) * pVector->x + (FixedPointNumberProduct)(xy - sz) * pVector->y + (FixedPointNumberProduct)(zx + sy) * pVector->z) / FIXED_POINT_NUMBER_SCALE;
    pResult->y = ((FixedPointNumberProduct)(ConvertToFixedPointNumber(1) - zz - xx) * pVector->y + (FixedPointNumberProduct)(yz - sx) * pVector->z + (FixedPointNumberProduct)(xy + sz) * pVector->x) / FIXED_POINT_NUMBER_SCALE;
    pResult->z = ((FixedPointNumberProduct)(ConvertToFixedPointNumber(1) - xx - yy) * pVector->z + (FixedPointNumberProduct)(zx - sy) * pVector->x + (FixedPointNumberProduct)(yz + sx) * pVector->y) / FIXED_POINT_NUMBER_SCALE;
}
*/
