#include "bsp_adc.h"

/*
1.内部ADC初始化包括温度计
2.四个ADC通道采集模拟传感器的值
*/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//Ch10---->PC0
//Ch11---->PC1
//Ch12---->PC2
//Ch13---->PC3
void  adc_Init(void)
{   
		//初始化IO
		RCC->APB2ENR |= 1<<4;    //使能PORTC口时钟 
		GPIOC->CRL &= 0XFFFF0000;//PC0 PC1 PC2	PC3	anolog输入

		//通道10		 
		RCC->APB2ENR |= 1<<9;    //ADC1时钟使能	  
		RCC->APB2RSTR |= 1<<9;   //ADC1复位
		RCC->APB2RSTR &= ~(1<<9);//复位结束	    
		RCC->CFGR &= ~(3<<14);   //分频因子清零	
		//SYSCLK/DIV2=9M ADC时钟设置为9M,ADC最大时钟不能超过14M!否则将导致ADC准确度下降! 
		RCC->CFGR |= 3<<14;      	 
		ADC1->CR1 &= 0XF0FFFF;   //工作模式清零
		ADC1->CR1 |= 0<<16;      //独立工作模式  
		ADC1->CR1 &= ~(1<<8);    //非扫描模式	  
		ADC1->CR2 &= ~(1<<1);    //单次转换模式
		ADC1->CR2 &= ~(7<<17);	   
		ADC1->CR2 |= 7<<17;	   //软件控制转换  
		ADC1->CR2 |= 1<<20;      //使用用外部触发(EXTTRIG)!!!	必须使用一个事件来触发
		ADC1->CR2 &= ~(1<<11);   //右对齐	 
		ADC1->CR2 |= 1<<23;      //使能温度传感器

		ADC1->SQR1 &= ~(0XF<<20);
		ADC1->SQR1 &= 0<<20;     //1个转换在规则序列中 也就是只转换规则序列1 			   
		//设置通道10的采样时间
		ADC1->SMPR1 &= ~(0x1F);  //通道10、11、12、13采样时间清空	  
		ADC1->SMPR1 |= 0x1F;     //通道10、11、12、13的采样周期239.5周期,提高采样时间可以提高精确度	 

		ADC1->SMPR1 &= ~(7<<18);  //清除通道16原来的设置	 
		ADC1->SMPR1 |= 7<<18;     //通道16  239.5周期,提高采样时间可以提高精确度	 

		ADC1->CR2 |= 1<<0;	   //开启AD转换器	 
		ADC1->CR2 |= 1<<3;       //使能复位校准  
		while(ADC1->CR2&1<<3); //等待校准结束 			 
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
		ADC1->CR2|=1<<2;        //开启AD校准	   
		while(ADC1->CR2&1<<2);  //等待校准结束
	//该位由软件设置以开始校准，并在校准结束时由硬件清除  
}				  

//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_adc(u8 ch)   
{
		//设置转换序列	  		 
		ADC1->SQR3 &= 0XFFFFFFE0;//规则序列1 通道ch
		ADC1->SQR3 |= ch;		  			    
		ADC1->CR2 |= 1<<22;       //启动规则转换通道 
		while(!(ADC1->SR & 1<<1));//等待转换结束	 	   
		return ADC1->DR;		    //返回adc值	
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
		u32 temp_val=0;
		u8 t;
		for(t=0;t<times;t++)
		{
				temp_val+=Get_adc(ch);
		}
		return temp_val/times;
} 
	 
//得到ADC采样内部温度传感器的温度值
//返回值3位温度值 XXX*0.1C	 
int Get_Temp(void)
{				 
		u16 temp_val=0;
		u8 t;
		float temperate;   
		for(t=0;t<20;t++)//读20次,取平均值
		{
				temp_val += Get_adc(ADC_CH_TEMP);
		}
		temp_val/=20;
		temperate=(float)temp_val*(3.3/4096);//得到温度传感器的电压值
		temperate=(1.43-temperate)/0.0043+25;//计算出当前温度值	 
		temperate*=10;//扩大十倍,使用小数点后一位
		return (int)temperate;	 

}

/* --------------------------------------end of file--------------------------------------- */