#ifndef __Algorithm_math_H
#define	__Algorithm_math_H

#include "stm32f10x.h"

#define M_PI  (float)3.1415926535
#define squa( Sq )        (((float)Sq)*((float)Sq))
#define toRad( Math_D )	  ((float)(Math_D)*0.0174532925f)
#define toDeg( Math_R )	  ((float)(Math_R)*57.2957795f)
#define absu16( Math_X )  (Math_X<0? -Math_X:Math_X)
#define absFloat( Math_X )(Math_X<0? -Math_X:Math_X)


float COS(float x);
float SIN(float y);
float Q_rsqrt(float number);
float VariableParameter(float error);
void array_assign(s16 *array,s16 value);
void array_astrict(s16 *array,s16 lower,s16 upper);
void data_limit(float data,float toplimit,float lowerlimit);

#endif /* __Algorithm_math_H */
