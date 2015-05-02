/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * 文件名  ：CONTROL.c
 * 描述    ：电机控制         
 * 实验平台：HT飞控
 * 库版本  ：ST3.5.0
 * 作者    ：Air Nano Team 
**********************************************************************************/
#include "include.h"

struct _ctrl ctrl;
u8 ARMED = 0;
vs16 Moto_duty[4];
/***************************************************/
/*void Control(float rol, float pit, float yaw)    */
/*输入：rol   横滚角                               */
/*      pit   俯仰角                               */
/*		  yaw   航向角                               */
/*输出：                                           */
/*备注：串级PID 控制   外环（角度环）采用PID调节   */
/*                     内环（角速度环）采用PD调节  */
/***************************************************/
void CONTROL(float rol, float pit, float yaw)
{
	static float roll_old,pitch_old;
	
	if(ctrl.ctrlRate >= 2)  //内环进行2次控制   外环进行1次控制   内环控制频率为外环的2倍 
	{
		//*****************外环PID**************************//
		//俯仰计算//
		pit=pit - (Rc_Data.PITCH - Rc_Data.pitch_offset)/20;
		ctrl.pitch.shell.increment += pit;   //俯仰方向误差积分
			
			//积分限幅
		if(ctrl.pitch.shell.increment > ctrl.pitch.shell.increment_max)
				ctrl.pitch.shell.increment = ctrl.pitch.shell.increment_max;
		else if(ctrl.pitch.shell.increment < -ctrl.pitch.shell.increment_max)
				ctrl.pitch.shell.increment = -ctrl.pitch.shell.increment_max;
		
		ctrl.pitch.shell.pid_out = ctrl.pitch.shell.kp * pit + ctrl.pitch.shell.ki * ctrl.pitch.shell.increment + ctrl.pitch.shell.kd * (pit - pitch_old);
		pitch_old = pit; //储存 俯仰偏差
		
		//横滚计算//
		rol=rol - (Rc_Data.ROLL - Rc_Data.roll_offset)/20;
		ctrl.roll.shell.increment += rol;  //横滚方向误差积分
			
			//积分限幅
		if(ctrl.roll.shell.increment > ctrl.roll.shell.increment_max)
				ctrl.roll.shell.increment = ctrl.roll.shell.increment_max;
		else if(ctrl.roll.shell.increment < -ctrl.roll.shell.increment_max)
				ctrl.roll.shell.increment = -ctrl.roll.shell.increment_max;

		ctrl.roll.shell.pid_out  = ctrl.roll.shell.kp * rol + ctrl.roll.shell.ki * ctrl.roll.shell.increment + ctrl.roll.shell.kd * (rol - roll_old);
		roll_old = rol;  //储存 横滚偏差

    //航向计算////////////
    ctrl.yaw.shell.pid_out = ctrl.yaw.shell.kp * (Rc_Data.YAW - Rc_Data.yaw_offset)/10 + ctrl.yaw.shell.kd * sensor.gyro.origin.z;		
		 ctrl.ctrlRate = 0;
	}
	ctrl.ctrlRate ++;
	
  //********************内环(角速度环)PD*********************************//
	ctrl.roll.core.kp_out = ctrl.roll.core.kp * (ctrl.roll.shell.pid_out + sensor.gyro.radian.y * RtA);  
	ctrl.roll.core.kd_out = ctrl.roll.core.kd * (sensor.gyro.origin.y - sensor.gyro.histor.y);
	
	ctrl.pitch.core.kp_out = ctrl.pitch.core.kp * (ctrl.pitch.shell.pid_out + sensor.gyro.radian.x * RtA);
	ctrl.pitch.core.kd_out = ctrl.pitch.core.kd * (sensor.gyro.origin.x - sensor.gyro.histor.x);
	
	ctrl.yaw.core.kp_out = ctrl.yaw.core.kp * (ctrl.yaw.shell.pid_out + sensor.gyro.radian.z * RtA);
	ctrl.yaw.core.kd_out = ctrl.yaw.core.kd * (sensor.gyro.origin.z - sensor.gyro.histor.z);
	
	ctrl.roll.core.pid_out = ctrl.roll.core.kp_out + ctrl.roll.core.kd_out;
	ctrl.pitch.core.pid_out = ctrl.pitch.core.kp_out + ctrl.pitch.core.kd_out;
	ctrl.yaw.core.pid_out =  ctrl.yaw.core.kp_out + ctrl.yaw.core.kd_out;

	sensor.gyro.histor.x = sensor.gyro.origin.x;   //储存陀螺仪（角速度）历史值
	sensor.gyro.histor.y = sensor.gyro.origin.y;
  sensor.gyro.histor.z = sensor.gyro.origin.z;
  

	if(Rc_Data.THROTTLE>1100)  
	{
		int date_THROTTLE	= Rc_Data.THROTTLE;///cos(angle.roll/57.3)/cos(angle.pitch/57.3);  //油门倾角补偿，防止因倾斜高度下降太快
		
		/*         控制采用X模式          */
		/*           1     4              */
		/*            \   /               */ 
		/*             \ /                */
		/*             / \                */
		/*            /   \               */
		/*           2     3              */
		/* 1:Moto_duty[0]  2:Moto_duty[1] */
    /* 3:Moto_duty[2]  4:Moto_duty[3] */
		
		Moto_duty[0] = date_THROTTLE - 1000 - ctrl.roll.core.pid_out - ctrl.pitch.core.pid_out - ctrl.yaw.core.pid_out;
		Moto_duty[1] = date_THROTTLE - 1000 - ctrl.roll.core.pid_out + ctrl.pitch.core.pid_out + ctrl.yaw.core.pid_out;
		Moto_duty[2] = date_THROTTLE - 1000 + ctrl.roll.core.pid_out + ctrl.pitch.core.pid_out - ctrl.yaw.core.pid_out;
		Moto_duty[3] = date_THROTTLE - 1000 + ctrl.roll.core.pid_out - ctrl.pitch.core.pid_out + ctrl.yaw.core.pid_out;
		
		if(Moto_duty[0]<=0) Moto_duty[0] = 0;
		if(Moto_duty[1]<=0) Moto_duty[1] = 0;
		if(Moto_duty[2]<=0) Moto_duty[2] = 0;
		if(Moto_duty[3]<=0) Moto_duty[3] = 0;
	}
	else
	{																																																																																																																																																																																																																																																																																																																																																																																																																																
		 Moto_duty[0] = Moto_duty[1] = Moto_duty[2] = Moto_duty[3] = 0;
		 ctrl.pitch.shell.increment = 0;
		 ctrl.roll.shell.increment = 0;
	}
	if(ARMED)  Moto_PwmRflash(Moto_duty[0],Moto_duty[1],Moto_duty[2],Moto_duty[3]);		
	else    Moto_PwmRflash(0,0,0,0);		
}



/*    油门形成设置    */
void ALGH_set(void)
{
	static vs8 flag=1,i;
	static vs16 time=0;
	for(i=0;i<100;i++)
	{
		 RC_Data_Refine();
		 delay_ms(2);
	}
  while(flag)
  {
		/*             油门形成设置               */
	  /*     —————————            —————————     */
	  /*    |    |    |          |         |    */
	  /*    |    |    |          |         |    */
	  /*    |         |          |     \   |    */
	  /*    |         |          |      \  |    */
	  /*     —————————            —————————     */
	  /*   油门拉到最高         摇杆推到右下角  */
		RC_Data_Refine();
  	if(Rc_Data.ROLL >= 1750 && Rc_Data.PITCH >= 1750)
		{
      if(Rc_Data.THROTTLE <= 1200 && Rc_Data.THROTTLE >= 900)
			{
        flag=0;  
      }
			Moto_PwmRflash(Rc_Data.THROTTLE-750 ,Rc_Data.THROTTLE -750,Rc_Data.THROTTLE -750,Rc_Data.THROTTLE-750);
			time=0;
    }
		else  time++;
    
    if(time>=30000)  flag=0; 		
		LED3(ON);
  }
	Moto_PwmRflash(0,0,0,0);
	LED3(OFF);

}

/*    上锁&解锁函数  */
void Deblocking(void)
{
	 static vs8 flag=1;
	 static vs16 time1=0,time2=0;
	 /*               遥控解锁                 */
	 /*     —————————            —————————     */
	 /*    |         |          |  \      |    */
	 /*    |         |          |   \     |    */
	 /*    |    |    |          |         |    */
	 /*    |    |    |          |         |    */
	 /*     —————————            —————————     */
	 /*   油门拉到最低         摇杆推到左上角  */
   if(!ARMED && Rc_Data.ROLL <= 1200 && Rc_Data.PITCH <= 1200 && Rc_Data.THROTTLE <= 1200 &&  Rc_Data.THROTTLE > 900)		{  time1++; }	
	 else time1=0;
	 if(time1>30 && !ARMED) { ARMED = 1; time1 = 0;}
   /*               遥控上锁                 */
	 /*     —————————            —————————     */
	 /*    |         |          |       / |    */
	 /*    |         |          |      /  |    */
	 /*    |    |    |          |         |    */
	 /*    |    |    |          |         |    */
	 /*     —————————            —————————     */
	 /*   油门拉到最低         摇杆推到右上角  */
   if(ARMED && Rc_Data.ROLL >= 1800 && Rc_Data.PITCH <= 1200 && Rc_Data.THROTTLE <= 1200 &&  Rc_Data.THROTTLE > 900)		{  time2++; }	
	 else time2=0;
	 if(time2>30 && ARMED)   {  ARMED = 0; time2 = 0;}
}

