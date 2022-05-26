#include "cdce813-q1.h"
#include "delay.h"
#include "usart.h"

void CDCE813Q1_Init(void)//��ʼ��CDCE813-Q1�е�IIC����
{
	IIC_Init();//IIC��ʼ��
}
void CDCE813Q1_WriteOneByte(u16 WriteAddr,u8 DataToWrite)//CDCE813-Q1дһ���ֽں���
{			
//	printf("1..%d  , 2.. %d \r\n" , 0XCA+((WriteAddr/256)<<1) ,WriteAddr%256 ) ; 
	IIC_Start();  
	IIC_Send_Byte(0Xca);   //����������ַ0XCA,д���� 
 	IIC_Wait_Ack(); 	 										  		   	
	IIC_Send_Byte(0X80+WriteAddr);   //���͵͵�ַ	 
	IIC_Wait_Ack(); 	 										  		   	
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack(); 	 										  		   	    	   
   IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

u8 CDCE813Q1_ReadOneByte(u16 ReadAddr)//CDCE813-Q1��һ���ֽں���
{	
	u8 temp = 0;
   IIC_Start();  	
	IIC_Send_Byte(0Xca);
	IIC_Wait_Ack(); 
	IIC_Send_Byte((0X80+ReadAddr)%256);
	IIC_Wait_Ack(); //����Ϊαд����
		    
	IIC_Start();//��ʼ���Ͷ���ʼ�ź�   /
	IIC_Send_Byte(0Xcb);           //�������ģʽ
	IIC_Wait_Ack(); 
   temp = IIC_Read_Byte(0);
	IIC_Ack();
   IIC_Stop();//����һ��ֹͣ����
	printf( "yyyy: %d \r\n",temp ) ;
	return temp;
}
void CDCE813Q1_Read(u16 ReadAddr,u8 *pBuffer)//CDCE813-Q1����������
{
	u8 temp;	
  IIC_Start();  	
	IIC_Send_Byte(0XCA);    
	IIC_Wait_Ack(); 
  IIC_Send_Byte((0X80+ReadAddr)%256);
	IIC_Wait_Ack();
	//����Ϊαд����	    
	IIC_Start();//��ʼ���Ͷ���ʼ�ź�   
	IIC_Send_Byte(0XCB);           //�������ģʽ			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);	//�õ��������ܵ��ֽڸ���	   
	while(temp)
	{
		*pBuffer++=IIC_Read_Byte(0);
		printf("%x",*pBuffer);
		temp--;
	}
	IIC_Stop();//����һ��ֹͣ����
}  
void CDCE813Q1_Write(u16 WriteAddr,u8 *pBuffer,u8 NumToWrite)//CDCE813-Q1����д����
{
	IIC_Start();  
	IIC_Send_Byte(0XCA);   //����������ַ0XCA,д���� 	   	  
	IIC_Wait_Ack();	   
  IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(NumToWrite);     //�����ֽڸ���					   
	IIC_Wait_Ack();  		    	   
	while(NumToWrite--)
	{
		IIC_Send_Byte(pBuffer[8-NumToWrite]);
		IIC_Wait_Ack();  	
	}
	IIC_Stop();//����һ��ֹͣ���� 
}

