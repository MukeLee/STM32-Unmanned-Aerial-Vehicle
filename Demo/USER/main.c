/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * �ļ���  ��main.c
 * ����    ��ϵͳ��ʼ��         
 * ʵ��ƽ̨��HT�ɿ�
 * ��汾  ��ST3.5.0
 * ����    ��Air Nano Team 
**********************************************************************************/
#include "include.h" 

extern u8  sentDateFlag;
 
int main(void)
{   
	  IAC_Init();
	  ALGH_set();
	  Sensor_Init();  //������ ��ʼ��      
	  paramLoad();
	  State_Display();//OLED������ʾ
	  ALGH_set();   //���������γ�
	  EnTIM3();       //����ʱ��
    while(1)
	  {     
			if(sentDateFlag)  //10MS����λ������һ������
			{
				sentDateFlag = 0;
         		UART1_ReportIMU();  //���ڷ�����̬
			}
			BATTDispaly(); //��ѹ��ʾ
	  }
}
/******************* (C) COPYRIGHT 2014 BEYOND *****END OF FILE************/

