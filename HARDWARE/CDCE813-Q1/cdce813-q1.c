#include "cdce813-q1.h"
#include "delay.h"
#include "usart.h"

void CDCE813Q1_Init(void)//初始化CDCE813-Q1中的IIC总线
{
	IIC_Init();//IIC初始化
}
void CDCE813Q1_WriteOneByte(u16 WriteAddr,u8 DataToWrite)//CDCE813-Q1写一个字节函数
{			
//	printf("1..%d  , 2.. %d \r\n" , 0XCA+((WriteAddr/256)<<1) ,WriteAddr%256 ) ; 
	IIC_Start();  
	IIC_Send_Byte(0Xca);   //发送器件地址0XCA,写数据 
 	IIC_Wait_Ack(); 	 										  		   	
	IIC_Send_Byte(0X80+WriteAddr);   //发送低地址	 
	IIC_Wait_Ack(); 	 										  		   	
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack(); 	 										  		   	    	   
   IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}

u8 CDCE813Q1_ReadOneByte(u16 ReadAddr)//CDCE813-Q1读一个字节函数
{	
	u8 temp = 0;
   IIC_Start();  	
	IIC_Send_Byte(0Xca);
	IIC_Wait_Ack(); 
	IIC_Send_Byte((0X80+ReadAddr)%256);
	IIC_Wait_Ack(); //以上为伪写操作
		    
	IIC_Start();//开始发送读开始信号   /
	IIC_Send_Byte(0Xcb);           //进入接收模式
	IIC_Wait_Ack(); 
   temp = IIC_Read_Byte(0);
	IIC_Ack();
   IIC_Stop();//产生一个停止条件
	printf( "yyyy: %d \r\n",temp ) ;
	return temp;
}
void CDCE813Q1_Read(u16 ReadAddr,u8 *pBuffer)//CDCE813-Q1连续读函数
{
	u8 temp;	
  IIC_Start();  	
	IIC_Send_Byte(0XCA);    
	IIC_Wait_Ack(); 
  IIC_Send_Byte((0X80+ReadAddr)%256);
	IIC_Wait_Ack();
	//以上为伪写操作	    
	IIC_Start();//开始发送读开始信号   
	IIC_Send_Byte(0XCB);           //进入接收模式			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);	//得到即将接受的字节个数	   
	while(temp)
	{
		*pBuffer++=IIC_Read_Byte(0);
		printf("%x",*pBuffer);
		temp--;
	}
	IIC_Stop();//产生一个停止条件
}  
void CDCE813Q1_Write(u16 WriteAddr,u8 *pBuffer,u8 NumToWrite)//CDCE813-Q1连续写函数
{
	IIC_Start();  
	IIC_Send_Byte(0XCA);   //发送器件地址0XCA,写数据 	   	  
	IIC_Wait_Ack();	   
  IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(NumToWrite);     //发送字节个数					   
	IIC_Wait_Ack();  		    	   
	while(NumToWrite--)
	{
		IIC_Send_Byte(pBuffer[8-NumToWrite]);
		IIC_Wait_Ack();  	
	}
	IIC_Stop();//产生一个停止条件 
}

