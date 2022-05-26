#include "key.h"
#include "delay.h"
#include "usart.h"

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;    //PA15按键key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);         //初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5按键key0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0按键wake	up
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉输入	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.
} 

void  Input_Lie() 
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11 ;    //PA15按键key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure); 

}
void  Output_Hang()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;    //PA15按键key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB,  &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
}

void  Input_Hang()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;    //PA15按键key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 		 //推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
}
void  Output_Lie() 
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11 ;    //PA15按键key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);	
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8  key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
	return 0;// 无按键按下
}


int read_key_num()//读取矩阵键盘的输入
{
	int xx = 0 , yy = 0 , num = 0 ; 
	Input_Lie() ; 
		Output_Hang();
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0 )
			{
				xx = 1 ; 
				printf("a\r\n");
			}
		}
		
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0 )
			{
				xx = 2 ; 
				printf("b\r\n");
			}
		}
		
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0 )
			{
				xx = 3 ; 
				printf("c\r\n");
			}
		}
		
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0 )
			{
				xx = 4 ; 
				printf("d\r\n");
			}
		}	
	
		Output_Lie() ; 
		Input_Hang();
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0 )
			{
				yy = 1 ; 
				printf("1\r\n");
			}
		}
		
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0 )
			{
				yy = 2 ; 
				printf("2\r\n");
			}
		}
		
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0 )
			{
				yy = 3 ; 
				printf("3\r\n");
			}
		}
		
		if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0 )
		{
			delay_ms(20);
			if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0 )
			{
				yy = 4 ; 
				printf("4\r\n");
			}
		}
		if(xx!=0&&yy!=0)
		{
				num = (xx-1)*4+yy ;	
		}
		xx = 0;
		yy = 0;
		return num;
}
