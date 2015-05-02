/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * �ļ���  ��CONTROL.c
 * ����    ���������         
 * ʵ��ƽ̨��HT�ɿ�
 * ��汾  ��ST3.5.0
 * ����    ��Air Nano Team 
**********************************************************************************/
#include "include.h"

struct _ctrl ctrl;
u8 ARMED = 0;
vs16 Moto_duty[4];
/***************************************************/
/*void Control(float rol, float pit, float yaw)    */
/*���룺rol   �����                               */
/*      pit   ������                               */
/*		  yaw   �����                               */
/*�����                                           */
/*��ע������PID ����   �⻷���ǶȻ�������PID����   */
/*                     �ڻ������ٶȻ�������PD����  */
/***************************************************/
void CONTROL(float rol, float pit, float yaw)
{
	static float roll_old,pitch_old;
	
	if(ctrl.ctrlRate >= 2)  //�ڻ�����2�ο���   �⻷����1�ο���   �ڻ�����Ƶ��Ϊ�⻷��2�� 
	{
		//*****************�⻷PID**************************//
		//��������//
		pit=pit - (Rc_Data.PITCH - Rc_Data.pitch_offset)/20;
		ctrl.pitch.shell.increment += pit;   //��������������
			
			//�����޷�
		if(ctrl.pitch.shell.increment > ctrl.pitch.shell.increment_max)
				ctrl.pitch.shell.increment = ctrl.pitch.shell.increment_max;
		else if(ctrl.pitch.shell.increment < -ctrl.pitch.shell.increment_max)
				ctrl.pitch.shell.increment = -ctrl.pitch.shell.increment_max;
		
		ctrl.pitch.shell.pid_out = ctrl.pitch.shell.kp * pit + ctrl.pitch.shell.ki * ctrl.pitch.shell.increment + ctrl.pitch.shell.kd * (pit - pitch_old);
		pitch_old = pit; //���� ����ƫ��
		
		//�������//
		rol=rol - (Rc_Data.ROLL - Rc_Data.roll_offset)/20;
		ctrl.roll.shell.increment += rol;  //�������������
			
			//�����޷�
		if(ctrl.roll.shell.increment > ctrl.roll.shell.increment_max)
				ctrl.roll.shell.increment = ctrl.roll.shell.increment_max;
		else if(ctrl.roll.shell.increment < -ctrl.roll.shell.increment_max)
				ctrl.roll.shell.increment = -ctrl.roll.shell.increment_max;

		ctrl.roll.shell.pid_out  = ctrl.roll.shell.kp * rol + ctrl.roll.shell.ki * ctrl.roll.shell.increment + ctrl.roll.shell.kd * (rol - roll_old);
		roll_old = rol;  //���� ���ƫ��

    //�������////////////
    ctrl.yaw.shell.pid_out = ctrl.yaw.shell.kp * (Rc_Data.YAW - Rc_Data.yaw_offset)/10 + ctrl.yaw.shell.kd * sensor.gyro.origin.z;		
		 ctrl.ctrlRate = 0;
	}
	ctrl.ctrlRate ++;
	
  //********************�ڻ�(���ٶȻ�)PD*********************************//
	ctrl.roll.core.kp_out = ctrl.roll.core.kp * (ctrl.roll.shell.pid_out + sensor.gyro.radian.y * RtA);  
	ctrl.roll.core.kd_out = ctrl.roll.core.kd * (sensor.gyro.origin.y - sensor.gyro.histor.y);
	
	ctrl.pitch.core.kp_out = ctrl.pitch.core.kp * (ctrl.pitch.shell.pid_out + sensor.gyro.radian.x * RtA);
	ctrl.pitch.core.kd_out = ctrl.pitch.core.kd * (sensor.gyro.origin.x - sensor.gyro.histor.x);
	
	ctrl.yaw.core.kp_out = ctrl.yaw.core.kp * (ctrl.yaw.shell.pid_out + sensor.gyro.radian.z * RtA);
	ctrl.yaw.core.kd_out = ctrl.yaw.core.kd * (sensor.gyro.origin.z - sensor.gyro.histor.z);
	
	ctrl.roll.core.pid_out = ctrl.roll.core.kp_out + ctrl.roll.core.kd_out;
	ctrl.pitch.core.pid_out = ctrl.pitch.core.kp_out + ctrl.pitch.core.kd_out;
	ctrl.yaw.core.pid_out =  ctrl.yaw.core.kp_out + ctrl.yaw.core.kd_out;

	sensor.gyro.histor.x = sensor.gyro.origin.x;   //���������ǣ����ٶȣ���ʷֵ
	sensor.gyro.histor.y = sensor.gyro.origin.y;
  sensor.gyro.histor.z = sensor.gyro.origin.z;
  

	if(Rc_Data.THROTTLE>1100)  
	{
		int date_THROTTLE	= Rc_Data.THROTTLE;///cos(angle.roll/57.3)/cos(angle.pitch/57.3);  //������ǲ�������ֹ����б�߶��½�̫��
		
		/*         ���Ʋ���Xģʽ          */
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



/*    �����γ�����    */
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
		/*             �����γ�����               */
	  /*     ������������������            ������������������     */
	  /*    |    |    |          |         |    */
	  /*    |    |    |          |         |    */
	  /*    |         |          |     \   |    */
	  /*    |         |          |      \  |    */
	  /*     ������������������            ������������������     */
	  /*   �����������         ҡ���Ƶ����½�  */
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

/*    ����&��������  */
void Deblocking(void)
{
	 static vs8 flag=1;
	 static vs16 time1=0,time2=0;
	 /*               ң�ؽ���                 */
	 /*     ������������������            ������������������     */
	 /*    |         |          |  \      |    */
	 /*    |         |          |   \     |    */
	 /*    |    |    |          |         |    */
	 /*    |    |    |          |         |    */
	 /*     ������������������            ������������������     */
	 /*   �����������         ҡ���Ƶ����Ͻ�  */
   if(!ARMED && Rc_Data.ROLL <= 1200 && Rc_Data.PITCH <= 1200 && Rc_Data.THROTTLE <= 1200 &&  Rc_Data.THROTTLE > 900)		{  time1++; }	
	 else time1=0;
	 if(time1>30 && !ARMED) { ARMED = 1; time1 = 0;}
   /*               ң������                 */
	 /*     ������������������            ������������������     */
	 /*    |         |          |       / |    */
	 /*    |         |          |      /  |    */
	 /*    |    |    |          |         |    */
	 /*    |    |    |          |         |    */
	 /*     ������������������            ������������������     */
	 /*   �����������         ҡ���Ƶ����Ͻ�  */
   if(ARMED && Rc_Data.ROLL >= 1800 && Rc_Data.PITCH <= 1200 && Rc_Data.THROTTLE <= 1200 &&  Rc_Data.THROTTLE > 900)		{  time2++; }	
	 else time2=0;
	 if(time2>30 && ARMED)   {  ARMED = 0; time2 = 0;}
}

