#include "stm32f10x_gpio.h"
#include "key.h"



 /*
 * �������� Delay
 * ����  ������ȷ����ʱ
 * ����  ����ʱ������
 * ���  ����
 */
 void Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

void KEY_Init(void)        
{
   GPIO_InitTypeDef  KEY_GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(KEY_RCCEN, ENABLE); 	 //GPIO始终使能

   KEY_GPIO_InitStructure.GPIO_Pin = KEY1|KEY2|KEY3|KEY4|KEY5;
	 KEY_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 KEY_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(KEY_GPIO, &KEY_GPIO_InitStructure); 		           //配置OLED端口引脚为GPIO输出
}

 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(void)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(KEY_GPIO,KEY1) == KEY_ON ) 
	{	   
		Delay(4000);		
		if(GPIO_ReadInputDataBit(KEY_GPIO,KEY1) == KEY_ON )  
		{	 
			while(GPIO_ReadInputDataBit(KEY_GPIO,KEY1) == KEY_ON);   
			return 	KEY_ENTER;	 
		}
	}
	else if(GPIO_ReadInputDataBit(KEY_GPIO,KEY2) == KEY_ON ) 
	{	   
		Delay(4000);		
		if(GPIO_ReadInputDataBit(KEY_GPIO,KEY2) == KEY_ON )  
		{	 
			while(GPIO_ReadInputDataBit(KEY_GPIO,KEY2) == KEY_ON);   
			return 	KEY_UP;	 
		}
	}
	else if(GPIO_ReadInputDataBit(KEY_GPIO,KEY3) == KEY_ON ) 
	{	   
		Delay(4000);		
		if(GPIO_ReadInputDataBit(KEY_GPIO,KEY3) == KEY_ON )  
		{	 
			while(GPIO_ReadInputDataBit(KEY_GPIO,KEY3) == KEY_ON);   
			return 	KEY_DOWN;	 
		}
	}
	else if(GPIO_ReadInputDataBit(KEY_GPIO,KEY4) == KEY_ON ) 
	{	   
		Delay(4000);		
		if(GPIO_ReadInputDataBit(KEY_GPIO,KEY4) == KEY_ON )  
		{	 
			while(GPIO_ReadInputDataBit(KEY_GPIO,KEY4) == KEY_ON);   
			return 	KEY_LEFT;	 
		}
	}
	else if(GPIO_ReadInputDataBit(KEY_GPIO,KEY5) == KEY_ON ) 
	{	   
		Delay(4000);		
		if(GPIO_ReadInputDataBit(KEY_GPIO,KEY5) == KEY_ON )  
		{	 
			while(GPIO_ReadInputDataBit(KEY_GPIO,KEY5) == KEY_ON);   
			return 	KEY_RIGHT;	 
		}
	}
	else return KEY_OFF;
}
