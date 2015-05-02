#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 GPIO_SetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);	 // turn off all led
	 GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);	 // turn off all led
}

void LED_Sailing(int rate)
{
	static u16 r,g;
	switch(r)
  {
		case 1:  LED1(ON);  break;
		case 2:  LED1(OFF); break;
		case 3:  LED1(ON);  break;
		case 4:  LED1(OFF); break;
		default: LED1(OFF); break;
	}
	g++;
	if(g > rate)
	{ 
	  g=0;
		r++;
	}
	if(r > 25)	 r = 0;	
}
