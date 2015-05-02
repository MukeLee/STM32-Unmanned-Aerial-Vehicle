/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * 文件名  ：main.c
 * 描述    ：系统初始化         
 * 实验平台：HT飞控
 * 库版本  ：ST3.5.0
 * 作者    ：Air Nano Team 
**********************************************************************************/
#include "include.h" 

extern u8  sentDateFlag;
 
int main(void)
{   
	  IAC_Init();
	  ALGH_set();
	  Sensor_Init();  //传感器 初始化      
	  paramLoad();
	  State_Display();//OLED数据显示
	  ALGH_set();   //设置油门形成
	  EnTIM3();       //开定时器
    while(1)
	  {     
			if(sentDateFlag)  //10MS向上位机发送一次数据
			{
				sentDateFlag = 0;
         		UART1_ReportIMU();  //串口发送姿态
			}
			BATTDispaly(); //电压显示
	  }
}
/******************* (C) COPYRIGHT 2014 BEYOND *****END OF FILE************/

