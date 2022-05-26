#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"  
#include "myiic.h"
#include "setuphz.h"
#include "cdce813-q1.h"
#include "clock.h"

//�����̿���TI��˾��оƬCDCE813-Q1������·�ɷֱ�������230Mhz���9khz�ķ�������Y1,Y2,Y3��Y2,Y3����ﲻ��80khz���£�����Y1��������
//ͨ��IICЭ����оƬ�������ӣ��޸�оƬ�ļĴ����ﵽ����Ч��
//ÿһ·�������ͨ����������޸�Ƶ��
//������̶��壨PB8-PB15����

//		0       1       2       3
//		4       5       6       7
//		8       9       Y1����	Y1���
//    Y2����  Y2���	Y3����	Y3���

//ʹ�÷��������¶�Ӧͨ�������룬Ȼ����HZΪ��λ�������ݣ��ٰ��¶�Ӧͨ����������ɡ�
//�������ߣ����������ֹ�Դ
//ָ����ʦ��Ī��ʦ

u8 Open_CloY23 = 0 ;
u32 F_out1 = 1000000/100 ;  //��λ ��100hz     ����Y1�ĳ�ʼ���Ƶ��
u32 F_out2 = 1000000/100 ;  //��λ ��100hz    ����Y2�ĳ�ʼ���Ƶ��
u32 F_out3 = 1000000/100 ;  //��λ ��100hz    ����Y3�ĳ�ʼ���Ƶ��

int main(void)
{ 
	u8 key2 = 0;
	u32 input_hz = 0 ;                             //���������������
	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����2
	delay_init();	    	                           //��ʱ������ʼ��	  
	uart_init(115200);	 	                         //���ڳ�ʼ��Ϊ115200
	LED_Init();		                              	 //��ʼ����LED���ӵ�Ӳ���ӿ�
 	CLK_ENABLE();																	 //PA8��ʱ���źų�ʼ��
	KEY_Init();																	 	 //������ʼ��	
 	CDCE813Q1_Init();	 														 //оƬ��ʼ��
 	Setup_YHz(F_out1 , F_out2, F_out3);						 //�����ʼƵ��
	delay_ms(5);
	while(1)
	{
		key2 = read_key_num();
		
		if ( key2 == 11 ) //Y1׼����������
		{
			input_hz = 0 ; 
			key2 = 0;
			delay_ms(100);
			while( key2 != 12) //Y1�������
			{
				key2=read_key_num();
				if ( key2>=1 && key2<=10 )
				{
				input_hz = input_hz*10+(key2-1); 	
				key2 = 0 ;
				delay_ms(200) ; 
				}
			}
			F_out1 = input_hz/100 ;//���뵥λ��HZ�������Ĵ�������Ϊ100HZ��������Ҫ����100
			printf("�����������: %d \r\n" , input_hz) ; 
			Setup_Y1Hz(F_out1);
			delay_ms(10);
			key2 = 0 ;
		}	
		
		if ( key2 == 13 ) //Y2׼����������
		{
			input_hz = 0 ; 
			key2 = 0;
			delay_ms(100);
			while( key2 != 14) //Y2�������
			{
				key2=read_key_num();
				if ( key2>=1 && key2<=10 )
				{
				input_hz = input_hz*10+(key2-1); 	
				key2 = 0 ;
				delay_ms(200) ; 
				}
			}
			F_out2 = input_hz/100 ;//���뵥λ��HZ�������Ĵ�������Ϊ100HZ��������Ҫ����100
			printf("�����������: %d \r\n" , input_hz) ; 
			Setup_Y2Hz(F_out2);
			delay_ms(10);
			key2 = 0 ;
		}	
		
		if ( key2 == 15 ) //Y3׼����������
		{
			input_hz = 0 ; 
			key2 = 0;
			delay_ms(100);
			while( key2 != 16) //Y1�������
			{
				key2=read_key_num();
				if ( key2>=1 && key2<=10 )
				{
				input_hz = input_hz*10+(key2-1); 	
				key2 = 0 ;
				delay_ms(200) ; 
				}
			}
			F_out3 = input_hz/100 ;//���뵥λ��HZ�������Ĵ�������Ϊ100HZ��������Ҫ����100
			printf("�����������: %d \r\n" , input_hz) ; 
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
