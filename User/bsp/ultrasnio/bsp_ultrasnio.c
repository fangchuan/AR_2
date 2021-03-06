/*
*********************************************************************************************************
*
*	模块名称 : 超声波传感器驱动模块
*	文件名称 : bsp_ultrasnio.c
*	版    本 : V1.0
*	说    明 : 采用TIM8的PWM捕获功能来读取超声波ECHO脚的电平
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#include  "bsp_ultrasnio.h"
#include  "_apollorobot.h"
#include  "os.h"
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
uint8_t  TIM8CH4_CAPTURE_STA=1;  //边沿捕捉标志位，=1表示捕捉到了上升沿，=0表示捕捉到了下降沿
uint16_t TIM8CH4_Rise,TIM8CH4_Fall,TIM8_T;

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern _Ultrasnio ult;//当前测得距离，单位为cm

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************************************************
*	函 数 名: NVIC_TIM8_Configuration
*	功能说明: TIM8中断配置
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
static void NVIC_TIM8_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM8 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_ULTRASNIO_PP;//抢占优先级最低
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_ULTRASNIO_SP;//子优先级最低
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
*	函 数 名: TIM8_PWMInCap_Init
*	功能说明: PC9接收超声波返回高电平
*					 配置TIM8为输入捕获模式，不是PWM输入模式。
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
static void TIM8_PWMInCap_Init(void)
{	 
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_ICInitTypeDef  TIM8_ICInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//使能TIM8时钟
		
		//初始化TIM8，脉冲宽度为1us~65.535ms之间	 
		TIM_TimeBaseStructure.TIM_Period = 0xffff;     //设定计数器自动重装值 
		TIM_TimeBaseStructure.TIM_Prescaler = 71 ; 	//我的主频跑到72Mhz，预分频器72分频=1Mhz，1us计一个数   
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //不分频
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //初始化TIM8参数
	 
		//初始化TIM8输入捕获参数,PC9
		TIM8_ICInitStructure.TIM_Channel = TIM_Channel_4; 
  	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//先上升沿捕获
  	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM8_ICInitStructure.TIM_ICFilter = 0x0A;//IC1F=1010 滤掉1us以下脉冲宽度的干扰
  	TIM_ICInit(TIM8, &TIM8_ICInitStructure);
		
		TIM_ITConfig(TIM8,TIM_IT_CC4,ENABLE);//允许CC4IE边沿捕获中断	 
	
		NVIC_TIM8_Configuration();

		TIM_Cmd(TIM8,ENABLE ); 	//使能定时器8
}
/*********************************************************************************************************
*	函 数 名: Ultrasnio_Port_Init
*	功能说明: 超声波占用的GPIO初始化
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
static void Ultrasnio_Port_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Pin  = ULTRASNIO_ECHO;    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //参考ST手册：配置成浮空输入，用于检测上升沿或者下降沿  
	  GPIO_Init(ULTRASNIO_PORT, &GPIO_InitStructure);	 
	
		GPIO_InitStructure.GPIO_Pin  = ULTRASNIO_TRIG;    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //参考ST手册：配置成推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
		GPIO_Init(ULTRASNIO_PORT, &GPIO_InitStructure);	
}
/*********************************************************************************************************
*	函 数 名: Ultra_delay_us
*	功能说明: 超声波延时
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
static void Ultra_delay_us(void)
{
    volatile int i = 16;	 
    while (i)
        i--;
}
static void Ultra_delay_nus(int n)
{
			int i = n;
			while(i--)
			{
				Ultra_delay_us();
			}
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	函 数 名: Ultrasnio_Init
*	功能说明: 超声波初始化
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Ultrasnio_Init(void)
{
			Ultrasnio_Port_Init();
			TIM8_PWMInCap_Init();
}
/*********************************************************************************************************
*	函 数 名: Ultrasnio_update
*	功能说明: 触发一次Trig引脚，进行一次测距更新
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Ultrasnio_update(void)
{	  
     Ultrasnio_Trigger_H;
     Ultra_delay_nus(7);//示波器测量此时延时为40us
		 Ultrasnio_Trigger_L;
}

/*********************************************************************************************************
*	函 数 名: TIM8_CC_IRQHandler
*	功能说明: TIM8中断服务函数
*          超声波ECHO引脚根据接受到的高电平时间更新距离
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/

void TIM8_CC_IRQHandler(void)
{  //进入中断
	 CPU_SR_ALLOC();
	 CPU_CRITICAL_ENTER();
	 OSIntEnter();
	 CPU_CRITICAL_EXIT();
	
	  if (TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET)//ch4发生捕获事件
		{	
				TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);   //清除中断标志位

				if(TIM8CH4_CAPTURE_STA == 1)	//捕获到上升沿
				{ 
						TIM8CH4_Rise = TIM_GetCapture4(TIM8);		           //获取上升沿的数据	  
						TIM8CH4_CAPTURE_STA = 0;		                        //转换标志位为下降沿
						TIM_OC4PolarityConfig(TIM8,TIM_ICPolarity_Falling);		//设置为下降沿捕获	  			    			  
				}
				else  						    //捕获到下降沿
				{
						TIM8CH4_Fall = TIM_GetCapture4(TIM8);	      //获取下降沿的数据	  
						TIM8CH4_CAPTURE_STA = 1;		//转换标志位为上升沿
						if(TIM8CH4_Fall < TIM8CH4_Rise)     //情况1：表示高电平跨过了65535这个特殊值，此时状态为上升沿接近65535，并且下降沿超过了65535从0开始计算，Tim8有溢出
						{
							TIM8_T = 65535;
						}
						else  //情况2：表示正常情况，上升沿和下降沿都在0-65535之间，且下降沿数值>上升沿数值。
						{
							TIM8_T = 0;
						}	
//						CPU_INT_DIS();
						ult.cur_distance = (float)(TIM8CH4_Fall - TIM8CH4_Rise + TIM8_T)*0.018;  //得到总的高电平时间，值域
//						CPU_INT_EN();
						TIM_OC4PolarityConfig(TIM8,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获		
				}		    
		}
		//退出中断
		OSIntExit();
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/
