#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"  
#include "myiic.h"
#include "setuphz.h"
#include "cdce813-q1.h"
#include "clock.h"

//本例程控制TI公司的芯片CDCE813-Q1生成三路可分别输出最高230Mhz最低9khz的方波脉冲Y1,Y2,Y3（Y2,Y3输出达不到80khz以下，仅有Y1能做到）
//通过IIC协议与芯片建立连接，修改芯片的寄存器达到控制效果
//每一路输出可以通过矩阵键盘修改频率
//矩阵键盘定义（PB8-PB15）：

//		0       1       2       3
//		4       5       6       7
//		8       9       Y1输入	Y1输出
//    Y2输入  Y2输出	Y3输入	Y3输出

//使用方法：按下对应通道的输入，然后以HZ为单位输入数据，再按下对应通道的输出即可。
//例程作者：赵利国，林国源
//指导老师：莫老师

u8 Open_CloY23 = 0 ;
u32 F_out1 = 1000000/100 ;  //单位 是100hz     定义Y1的初始输出频率
u32 F_out2 = 1000000/100 ;  //单位 是100hz    定义Y2的初始输出频率
u32 F_out3 = 1000000/100 ;  //单位 是100hz    定义Y3的初始输出频率

int main(void)
{ 
	u8 key2 = 0;
	u32 input_hz = 0 ;                             //矩阵键盘输入数据
	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组2
	delay_init();	    	                           //延时函数初始化	  
	uart_init(115200);	 	                         //串口初始化为115200
	LED_Init();		                              	 //初始化与LED连接的硬件接口
 	CLK_ENABLE();																	 //PA8口时钟信号初始化
	KEY_Init();																	 	 //按键初始化	
 	CDCE813Q1_Init();	 														 //芯片初始化
 	Setup_YHz(F_out1 , F_out2, F_out3);						 //输出初始频率
	delay_ms(5);
	while(1)
	{
		key2 = read_key_num();
		
		if ( key2 == 11 ) //Y1准备输入数据
		{
			input_hz = 0 ; 
			key2 = 0;
			delay_ms(100);
			while( key2 != 12) //Y1输出数据
			{
				key2=read_key_num();
				if ( key2>=1 && key2<=10 )
				{
				input_hz = input_hz*10+(key2-1); 	
				key2 = 0 ;
				delay_ms(200) ; 
				}
			}
			F_out1 = input_hz/100 ;//输入单位是HZ，传给寄存器定义为100HZ，所以需要除以100
			printf("输入的数据是: %d \r\n" , input_hz) ; 
			Setup_Y1Hz(F_out1);
			delay_ms(10);
			key2 = 0 ;
		}	
		
		if ( key2 == 13 ) //Y2准备输入数据
		{
			input_hz = 0 ; 
			key2 = 0;
			delay_ms(100);
			while( key2 != 14) //Y2输出数据
			{
				key2=read_key_num();
				if ( key2>=1 && key2<=10 )
				{
				input_hz = input_hz*10+(key2-1); 	
				key2 = 0 ;
				delay_ms(200) ; 
				}
			}
			F_out2 = input_hz/100 ;//输入单位是HZ，传给寄存器定义为100HZ，所以需要除以100
			printf("输入的数据是: %d \r\n" , input_hz) ; 
			Setup_Y2Hz(F_out2);
			delay_ms(10);
			key2 = 0 ;
		}	
		
		if ( key2 == 15 ) //Y3准备输入数据
		{
			input_hz = 0 ; 
			key2 = 0;
			delay_ms(100);
			while( key2 != 16) //Y1输出数据
			{
				key2=read_key_num();
				if ( key2>=1 && key2<=10 )
				{
				input_hz = input_hz*10+(key2-1); 	
				key2 = 0 ;
				delay_ms(200) ; 
				}
			}
			F_out3 = input_hz/100 ;//输入单位是HZ，传给寄存器定义为100HZ，所以需要除以100
			printf("输入的数据是: %d \r\n" , input_hz) ; 
			Setup_Y3Hz(F_out3);
			delay_ms(10);
			key2 = 0 ;
		}	
		
		t++;
		if(t>20)
		{
			t = 0;
			LED1 = !LED1 ; 
		}
		delay_ms(20) ;	
	}
	
}
