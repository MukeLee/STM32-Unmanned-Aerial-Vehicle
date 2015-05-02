#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "stm32f10x.h"
#include "include.h"

struct _pid{
        float kp;
			  float ki;
	      float kd;
	      float increment;
	      float increment_max;
	      float kp_out;
			  float ki_out;
	      float kd_out;
	      float pid_out;
          };

struct _tache{
    struct _pid shell;
    struct _pid core;	
          };
	

struct _ctrl{
		      u8  ctrlRate;
      struct _tache pitch;    
	    struct _tache roll;  
	    struct _tache yaw;   
            };

extern struct _ctrl ctrl;						
						
extern u8 ARMED;

void CONTROL(float rol, float pit, float yaw);
void PID_INIT(void);
void ALGH_set(void);
void Deblocking(void);

#endif
