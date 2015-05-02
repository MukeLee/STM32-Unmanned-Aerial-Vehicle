#ifndef __TIM_PWM_IN_H_
#define __TIM_PWM_IN_H_
#include "stm32f10x.h"

typedef struct int16_rcget{
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
	      int16_t pitch_offset;
	      int16_t roll_offset;
	      int16_t yaw_offset;}T_RC_DATA;

				
extern T_RC_DATA Rc_Data;//1000~2000


void TIM4_Cap_Init(void);
void RC_Data_Refine(void);
#endif
