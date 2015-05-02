/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "include.h"

u8  sentDateFlag=0;


void TIM3_IRQHandler(void)		    //2.5ms中断一次
{	
	static u16 ms1=0;	//中断次数计数器
	if(TIM3->SR & TIM_IT_Update)	
	{    
    TIM3->SR = ~TIM_FLAG_Update;//清除中断标志		
		ms1++;
    
		RC_Data_Refine();
		Prepare_Data();             //数据处理
 		Get_Attitude();             //姿态解算
		CONTROL(angle.pitch,angle.roll,0); //电机控制
		
		if(ms1==4)
		{
      ms1 =0;
			Deblocking();         //上锁&&解锁
			if(ARMED)	 LED_Sailing(5);
			sentDateFlag = 1;
    }
	}
}

u8 RxBUffer[50];
u8 RxState = 0;
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART3_IRQHandler(void)
{
	 if(USART3->SR & (1<<5))
   {	  	 
     u8 com_data = USART3->DR;
		 static u8 data_len = 0,data_cnt = 0;
		 if(RxState==0&&com_data==0x8A)
		 {
       RxState=1;
			 RxBUffer[0]=com_data;
     }			
  	 else if(RxState==1&&com_data==0x8B)
		 {
       RxState=2;
			 RxBUffer[1]=com_data;
     }		 
		 else if(RxState==2&&com_data==0x1C)
		 {
       RxState=3;
			 RxBUffer[2]=com_data;
			 data_len = 28;
			 data_cnt = 0;
     }	
     else if(RxState==3)		
		 {
			 data_len--;
       RxBUffer[3 + data_cnt++]=com_data;
			 if(data_len ==0)				RxState=4;				 
     }			 
		 else if(RxState==4)
		 {
       RxState=0;
			 ctrl.pitch.shell.kp = (float)((vs16)(RxBUffer[4]<<8)| RxBUffer[5])/100;
			 ctrl.pitch.shell.ki = (float)((vs16)(RxBUffer[6]<<8)| RxBUffer[7])/100;
			 ctrl.pitch.shell.kd = (float)((vs16)(RxBUffer[6]<<10)| RxBUffer[9])/1000;
			 ctrl.pitch.core.kp = (float)((vs16)(RxBUffer[10]<<8)| RxBUffer[11])/1000;
			 ctrl.pitch.core.kd = (float)((vs16)(RxBUffer[14]<<8)| RxBUffer[15])/1000;
     }
		 else  RxState=0; 
	} 
}
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
