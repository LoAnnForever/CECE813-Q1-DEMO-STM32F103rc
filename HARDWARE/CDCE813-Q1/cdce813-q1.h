#ifndef _CDCE813Q1_H
#define _CDCE813Q1_H
#include "sys.h"
#include "myiic.h"

void CDCE813Q1_Init(void); 																				//��ʼ��CDCE813-Q1�е�IIC����
void CDCE813Q1_WriteOneByte(u16 WriteAddr,u8 DataToWrite);				//CDCE813-Q1дһ���ֽں���	
u8 CDCE813Q1_ReadOneByte(u16 ReadAddr);														//CDCE813-Q1��һ���ֽں���	
void CDCE813Q1_Write(u16 WriteAddr,u8 *pBuffer,u8 NumToWrite);		//CDCE813-Q1����д����
void CDCE813Q1_Read(u16 ReadAddr,u8 *pBuffer); 										//CDCE813-Q1����������

#endif
