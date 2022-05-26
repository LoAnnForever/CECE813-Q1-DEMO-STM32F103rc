#ifndef _CDCE813Q1_H
#define _CDCE813Q1_H
#include "sys.h"
#include "myiic.h"

void CDCE813Q1_Init(void); 																				//初始化CDCE813-Q1中的IIC总线
void CDCE813Q1_WriteOneByte(u16 WriteAddr,u8 DataToWrite);				//CDCE813-Q1写一个字节函数	
u8 CDCE813Q1_ReadOneByte(u16 ReadAddr);														//CDCE813-Q1读一个字节函数	
void CDCE813Q1_Write(u16 WriteAddr,u8 *pBuffer,u8 NumToWrite);		//CDCE813-Q1连续写函数
void CDCE813Q1_Read(u16 ReadAddr,u8 *pBuffer); 										//CDCE813-Q1连续读函数

#endif
