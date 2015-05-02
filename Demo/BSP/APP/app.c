/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * �ļ���  ��INIT.c
 * ����    ��ϵͳ��ʼ��         
 * ʵ��ƽ̨��HT�ɿ�
 * ��汾  ��ST3.5.0
 * ����    ��Air Nano Team 
**********************************************************************************/
#include "include.h"
#include "app.h"

State Init_State;
float BATT[20];

void IAC_Init(void)
{
	 delay_init(72);  //��ʱ������ʼ��
	 Nvic_Init();     //�жϳ�ʼ�� 
	 LED_Init();      //led��ʼ��
	 OLED_Init();     //oled��ʼ��
	 Draw_LibLogo();  
	 KEY_Init();      //������ʼ��
	 ADC1_Init();     //AD��ʼ��
	 I2C_INIT();      //IIC��ʼ��
	 TIM4_Cap_Init(); //PWM���벶���ʼ��
   Moto_Init();		  //�����ʼ��
	 usart1_config(); //����1��ʼ�� 
	 usart3_config(); //����3��ʼ�� 
	 TIM3_Init(2000); //��ʱ����ʼ�� 2.5MS
	 FLASH_Unlock();
	 EE_Init();
}

void Sensor_Init(void)
{
	  NRF24L01_Init();
	 Init_State.MPU6050_State = InitMPU6050();      //MPU6050��ʼ��
	 Init_State.HMC5883_State = Init_HMC5883L();    //�شų�ʼ�� 
	 Init_State.MS5611_State  = MS5611_init();      //��ʼ����ѹ��
	 Init_State.NRF2401_State = NRF24L01_Check();   //���2401
	 NRF24L01_Mode(1);  //����2401Ϊ����ģʽ
	
}
void State_Display(void)
{
  OLED_Fill(0x00); //����
	if(Init_State.MPU6050_State)  OLED_P6x8Str(0,0,"MPU6050 is OK");
 	else                 OLED_P6x8Str(0,0,"MPU6050 is ERROR");
	if(Init_State.HMC5883_State)  OLED_P6x8Str(0,1,"HMC5883 is OK");
	else                 OLED_P6x8Str(0,1,"HMC5883 is ERROR");
	if(Init_State.MS5611_State)   OLED_P6x8Str(0,2,"MS5611  is OK");
	else                 OLED_P6x8Str(0,2,"MS5611  is ERROR");
	if(Init_State.NRF2401_State)  OLED_P6x8Str(0,3,"Nrf2401 is OK");
	else                 OLED_P6x8Str(0,3,"Nrf2401 is ERROR");
	
  OLED_P6x8Str(0,5,"YAW:");
	OLED_P6x8Str(0,6,"THR:");
	OLED_P6x8Str(56,5,"ROLL:");
	OLED_P6x8Str(56,6,"PITCH:"); 
}


void  BATTDispaly(void)
{
	static u8 ti=0;
	float Battery_voltage,sum = 0; 

	BATT[ti] = (float) ADC_ConvertedValue*11/4096*3.3;
	for(int i=0; i<20; i++)
	{
	  sum += BATT[i]; 
	}
	Battery_voltage = sum/20;
	ti++;
	if(ti == 20)  ti=0;
	
  OLED_P6x8Str(0,4,"VBATT:");
  Dis_Float(4,37,Battery_voltage,1);			//�����ص�ѹ
	OLED_P6x8Str(62,4,"V");
	
	OLED_4num(4,5,Rc_Data.YAW);
	OLED_4num(4,6,Rc_Data.THROTTLE);
	OLED_4num(57,5,Rc_Data.ROLL);
	OLED_4num(58,6,Rc_Data.PITCH);
}
