#include "TIM_PWM_IN.h"

T_RC_DATA Rc_Data;//1000~2000
u16  Rise[4],Drop[4];
u16  RC_Pwm_In[4];
u16  RC_Pwm_In_his[4];

void TIM4_Cap_Init(void)
{	 
	  GPIO_InitTypeDef         GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_ICInitTypeDef  TIM4_ICInitStructure;

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	 //ʹ��TIM4ʱ��
 	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //ʹ��GPIOBʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //ʹ��AFIO���ܵ�ʱ��
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);  //������ӳ��
	
	
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;             
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;            
	  GPIO_Init(GPIOD, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);		

	  //��ʼ����ʱ��4 TIM4	 
	  TIM_TimeBaseStructure.TIM_Period = 0XFFFF;                   //�趨�������Զ���װֵ 
	  TIM_TimeBaseStructure.TIM_Prescaler =71; 	                   //Ԥ��Ƶ��   
	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	  //��ʼ��TIM4���벶�����
	  TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1;                //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPolarity =TIM_ICPolarity_Rising;	   //�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	  TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;                //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
		
		TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3;                //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
		
		TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4;                //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	  TIM_Cmd(TIM4,ENABLE ); 
		
	  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);        //��������ж� ,����CC1IE�����ж�	
	  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	  TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	  TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
			
}





void TIM4_IRQHandler(void)
{ 
    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)   //����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); //����жϱ�־λ
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == 1) 
			{
				  TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
          Rise[0]=TIM_GetCapture1(TIM4);
      }
			else 
			{
				  TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
          Drop[0]=TIM_GetCapture1(TIM4);
				  if(Rise[0]>Drop[0])  RC_Pwm_In[0] = 65535-Rise[0] + Drop[0];
					else 	               RC_Pwm_In[0] = Drop[0] - Rise[0];
      }			
		}	
	 
	  if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)            //����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2); //����жϱ�־λ
	    if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) == 1) 
			{
				  TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
          Rise[1]=TIM_GetCapture2(TIM4);
      }
			else 
			{
				  TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
          Drop[1]=TIM_GetCapture2(TIM4);
				  if(Rise[1]>Drop[1])  RC_Pwm_In[1] = 65535-Rise[1] + Drop[1];
					else 	               RC_Pwm_In[1] = Drop[1] - Rise[1];
      }	
		}	
  	
    if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)            //����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3); //����жϱ�־λ
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) == 1) 
			{
				  TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
          Rise[2]=TIM_GetCapture3(TIM4);
      }
			else 
			{
				  TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
          Drop[2]=TIM_GetCapture3(TIM4);
				  if(Rise[2]>Drop[2]) RC_Pwm_In[2] = 65535-Rise[2] + Drop[2];
					else 	              RC_Pwm_In[2] = Drop[2] - Rise[2];
      }	 
		}	

    if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)            //����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4); //����жϱ�־λ
		  if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) == 1) 
			{
				  TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
          Rise[3]=TIM_GetCapture4(TIM4);
      }
			else 
			{
				  TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
          Drop[3]=TIM_GetCapture4(TIM4);
				  if(Rise[3]>Drop[3])  RC_Pwm_In[3] = 65535-Rise[3] + Drop[3];
					else 	               RC_Pwm_In[3] = Drop[3] - Rise[3];
      }	  
		}		
}

/*====================================================================================================*/
/*====================================================================================================*
**���� : RcData_Refine
**���� : ����ң������
**���� : None
**��� : None
**��ע : ��
**====================================================================================================*/
/*====================================================================================================*/
void RC_Data_Refine(void)
{
  u8 chan,a;	

	u16 rcDataMax[4], rcDataMin[4];
	static int16_t rcDataCache[4][4], rcDataMean[4];
	static uint8_t rcValuesIndex = 0;

	rcValuesIndex++;
	for (chan = 0; chan < 4; chan++) {
		  //����ƽ��ֵ�˲���4��
		  if(RC_Pwm_In[chan]>2800 || RC_Pwm_In[chan]<800)  RC_Pwm_In[chan] = RC_Pwm_In_his[chan];
			rcDataCache[chan][rcValuesIndex % 4] = RC_Pwm_In[chan] ;		
		  RC_Pwm_In_his[chan] = RC_Pwm_In[chan];
			
			rcDataMean[chan] = 0;
		  rcDataMax[chan]  = 0;
		  rcDataMin[chan]  = 25000;
		
			for (a = 0; a < 4; a++) {
				  // ��¼���������ֵ && ��Сֵ
				  if(rcDataCache[chan][a] > rcDataMax[chan])  rcDataMax[chan] = rcDataCache[chan][a];     
					if(rcDataCache[chan][a] < rcDataMin[chan])	rcDataMin[chan] = rcDataCache[chan][a]; 
				  // ���
					rcDataMean[chan] += rcDataCache[chan][a];  
      }
			// �޳������� ���ֵ && ��Сֵ 
			rcDataMean[chan] = (rcDataMean[chan] - (rcDataMax[chan] + rcDataMin[chan])) / 2;
	} 
	
	//for(chan=0;chan<6;chan++)
	 Rc_Data.YAW   = rcDataMean[0];
	 Rc_Data.THROTTLE  = rcDataMean[1];
	 Rc_Data.PITCH  = rcDataMean[2];
	 Rc_Data.ROLL = rcDataMean[3];
}

