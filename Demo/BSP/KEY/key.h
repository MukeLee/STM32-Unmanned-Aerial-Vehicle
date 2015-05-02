#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"
#define KEY_GPIO    GPIOE
#define KEY_RCCEN   RCC_APB2Periph_GPIOE
#define KEY1        GPIO_Pin_15      //D0
#define KEY2        GPIO_Pin_12      //D1
#define KEY3        GPIO_Pin_10      //RST
#define KEY4        GPIO_Pin_8       //DC
#define KEY5        GPIO_Pin_7       //DC

 /*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
 
#define KEY_OFF	0
#define KEY_ON	  1
#define KEY_ENTER	2
#define KEY_UP  	3
#define KEY_DOWN	4
#define KEY_LEFT	5
#define KEY_RIGHT	6

void KEY_Init(void);
uint8_t Key_Scan(void);
#endif
