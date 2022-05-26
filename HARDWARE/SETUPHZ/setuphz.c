#include "setuphz.h"
#include "clock.h"
#include "sys.h"
#include "cdce813-q1.h"
#include "math.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

u32 CLK_IN=80000;  //��������ʱ���źŵ�Ƶ�ʵ�λ��100Hz
void Setup_YHz(u32 hz , u32 Yout2 , u32 Yout3)//��·ͬʱ�޸����Ƶ��
{
	u16 i = 1 , Pdiv2 = 1 , Pdiv3 =1 ;
	u32 M, N, Pdiv, Q, R;
	u8 reg18, reg19, reg1A, reg1B;
 
	int P;
	u32 f_vco = hz;
  int result = 0;
	u8 f_range;

	while (f_vco < 800000 )
	{
		i++;		
		f_vco = hz * i;
	}

	while (f_vco <= 2300000 )
	{
		for (N = 4095; N > 0; N--)
		{
			for (M = 511; M > 0; M--)
			{
				if ((N * CLK_IN / M) == f_vco)
				{ 
						result = 1;
						break;				
				}
			}
			if (result)
			{
				break;
			}
		}
		if (result)
		{
			break;
		}
		else
		{
			i++;
			f_vco = hz * i;
		}
	}

	if (!result)
	{
		return;
	}
	
	P = 4 - (int)((log((double)N / (double)M))/log(2));//����P
	if (P < 0)
	{
		P = 0;
	}
	Q = (int)((double)N * pow(2, (double)P) / (double)M );//����Q
	R = (double)N * pow(2, (double)P) - M * Q;//����R
	if (f_vco < 1250000 )
	{
		f_range = 0;
	}
	else if ((f_vco >= 1250000 ) && (f_vco < 1500000 ))
	{
		f_range = 1;
	}
	else if ((f_vco >= 1500000 ) && (f_vco < 1750000 ))
	{
		f_range = 2;
	}
	else
	{
		f_range = 3;
	}
	S0 = 0;
	Pdiv = f_vco / hz;
	Pdiv2 = f_vco /Yout2;
	Pdiv3 = f_vco /Yout3;
	printf("Fvco = %d .... Pdiv = %d ...  Pdiv2 = %d ... Pdiv3 = %d ...\r\n" ,f_vco,Pdiv,Pdiv2  , Pdiv3 ) ; 
	printf("M = %d...N = %d....P = %d....R = %d...Q = %d...Pdiv = %d\r\n",M , N,P , R , Q , Pdiv);
	if(hz >= 800)
	{
		CDCE813Q1_Init();				    //��ʼ��IIC 
		delay_ms(10);
		//�趨Y1�ڵ�Ԥ��Ƶֵ
		CDCE813Q1_WriteOneByte(0x02, 0xB4+ (  (Pdiv>>8)&0x3 ) );
		CDCE813Q1_WriteOneByte(0x03, (u8)Pdiv);
		CDCE813Q1_WriteOneByte(0x04, 0x02);
		CDCE813Q1_WriteOneByte(0x05, 0x00);
		CDCE813Q1_WriteOneByte(0x06, 0x40);
		CDCE813Q1_WriteOneByte(0x12, 0x00);
		CDCE813Q1_WriteOneByte(0x13, 0x01);
		CDCE813Q1_WriteOneByte(0x14, 0x6D);//ʹ��PLL 0X6D
		CDCE813Q1_WriteOneByte(0x15, 0x02);
		//�趨Y2Y3�ڵ�Ԥ��Ƶֵ
		CDCE813Q1_WriteOneByte(0x16,  0X7f&(Pdiv2) );
		CDCE813Q1_WriteOneByte(0x17,  0X7f&(Pdiv3) );
		//д��N,P,R,Q,f_range��ֵ
		reg18 = (N >> 4) & 0xFF;
		reg19 = ( (N & 0x00f) << 4 )+ ((R & 0xf0) >> 5);
		reg1A = ((R & 0x1f) << 3 )+ ((Q >> 3) & 0x07);
		reg1B = ((Q & 0x07) << 5) + ( (P & 0x07) << 2 )+ (f_range & 0x03);
		CDCE813Q1_WriteOneByte(0x18, reg18);
		CDCE813Q1_WriteOneByte(0x19, reg19);
		CDCE813Q1_WriteOneByte(0x1A, reg1A);
		CDCE813Q1_WriteOneByte(0x1B, reg1B);
		CDCE813Q1_WriteOneByte(0x1C, N);
		CDCE813Q1_WriteOneByte(0x1D, ((N & 0xf) << 4) + (R & 0xf0));
		CDCE813Q1_WriteOneByte(0x1E, (R & 0x0f) + (Q & 0xf0));
		CDCE813Q1_WriteOneByte(0x1F, ((Q & 0x07) << 5) + ((P & 0x07) << 2) + (f_range & 0x03));
		S0 = 1;
	}
	else//���Ƶ�ʹ��ͣ����Y1�ڣ�������·PLL���
	{
		Pdiv = Pdiv/10;
		CDCE813Q1_Init();				    //��ʼ��IIC 
		delay_ms(10);
		//�趨Y1�ڵ�Ԥ��Ƶֵ
		CDCE813Q1_WriteOneByte(0x02, 0x34+ (  (Pdiv>>8)&0x3 ) );
		CDCE813Q1_WriteOneByte(0x03, (u8)Pdiv);
		CDCE813Q1_WriteOneByte(0x04, 0x02);
		CDCE813Q1_WriteOneByte(0x05, 0x00);
		CDCE813Q1_WriteOneByte(0x06, 0x40);
		CDCE813Q1_WriteOneByte(0x12, 0x00);
		CDCE813Q1_WriteOneByte(0x13, 0x01);
		CDCE813Q1_WriteOneByte(0x14, 0xED);//��·PLL 0XED
		CDCE813Q1_WriteOneByte(0x15, 0x02);
		//��·PLL��Y2Y3���޷��޸�����Ĵ���
		CDCE813Q1_WriteOneByte(0x16,  0x00 );
		CDCE813Q1_WriteOneByte(0x17,  0x00 );
		//д��N,P,R,Q,f_range��ֵ
		reg18 = (N >> 4) & 0xFF;
		reg19 = ( (N & 0x00f) << 4 )+ ((R & 0xf0) >> 5);
		reg1A = ((R & 0x1f) << 3 )+ ((Q >> 3) & 0x07);
		reg1B = ((Q & 0x07) << 5) + ( (P & 0x07) << 2 )+ (f_range & 0x03);
		CDCE813Q1_WriteOneByte(0x18, reg18);
		CDCE813Q1_WriteOneByte(0x19, reg19);
		CDCE813Q1_WriteOneByte(0x1A, reg1A);
		CDCE813Q1_WriteOneByte(0x1B, reg1B);
		CDCE813Q1_WriteOneByte(0x1C, N);
		CDCE813Q1_WriteOneByte(0x1D, ((N & 0xf) << 4) + (R & 0xf0));
		CDCE813Q1_WriteOneByte(0x1E, (R & 0x0f) + (Q & 0xf0));
		CDCE813Q1_WriteOneByte(0x1F, ((Q & 0x07) << 5) + ((P & 0x07) << 2) + (f_range & 0x03));
		S0 = 1;
	}
}
void Setup_Y1Hz(u32 hz)
{
	u16 i = 1;
	u32 M, N, Pdiv, Q, R;
	u8 reg18, reg19, reg1A, reg1B;
	int P;
	u32 f_vco = hz;
  int result = 0;
	u8 f_range;

	while (f_vco < 800000 )
	{
		i++;		
		f_vco = hz * i;
	}

	while (f_vco <= 2300000 )
	{
		for (N = 4095; N > 0; N--)
		{
			for (M = 511; M > 0; M--)
			{
				if ((N * CLK_IN / M) == f_vco)
				{ 
						result = 1;
						break;				
				}
			}
			if (result)
			{
				break;
			}
		}
		if (result)
		{
			break;
		}
		else
		{
			i++;
			f_vco = hz * i;
		}
	}

	if (!result)
	{
		return;
	}
	
	P = 4 - (int)((log((double)N / (double)M))/log(2));//����P
	if (P < 0)
	{
		P = 0;
	}
	Q = (int)((double)N * pow(2, (double)P) / (double)M );//����Q
	R = (double)N * pow(2, (double)P) - M * Q;//����R
	if (f_vco < 1250000 )
	{
		f_range = 0;
	}
	else if ((f_vco >= 1250000 ) && (f_vco < 1500000 ))
	{
		f_range = 1;
	}
	else if ((f_vco >= 1500000 ) && (f_vco < 1750000 ))
	{
		f_range = 2;
	}
	else
	{
		f_range = 3;
	}
	S0 = 0;
	Pdiv = f_vco / hz;
	printf("Fvco = %d .... Pdiv = %d  ...\r\n" ,f_vco,Pdiv) ; 
	printf("M = %d...N = %d....P = %d....R = %d...Q = %d...Pdiv = %d\r\n",M , N,P , R , Q , Pdiv);
	if(hz >= 800)
	{
		CDCE813Q1_Init();				    //��ʼ��IIC 
		delay_ms(10);
		// �趨Y1��pdiv1��Ԥ��Ƶֵ
		CDCE813Q1_WriteOneByte(0x02, 0xB4+ (  (Pdiv>>8)&0x3 ) );
		CDCE813Q1_WriteOneByte(0x03, (u8)Pdiv);
		//д��ͨ�üĴ���
		CDCE813Q1_WriteOneByte(0x04, 0x02);
		CDCE813Q1_WriteOneByte(0x05, 0x00);
		CDCE813Q1_WriteOneByte(0x06, 0x40);
		CDCE813Q1_WriteOneByte(0x12, 0x00);
		CDCE813Q1_WriteOneByte(0x13, 0x01);
		CDCE813Q1_WriteOneByte(0x14, 0x6D);
		CDCE813Q1_WriteOneByte(0x15, 0x02);
		//д��N,P,R,Q,�Լ�f_range��ֵ
		reg18 = (N >> 4) & 0xFF;
		reg19 = ( (N & 0x00f) << 4 )+ ((R & 0xf0) >> 5);
		reg1A = ((R & 0x1f) << 3 )+ ((Q >> 3) & 0x07);
		reg1B = ((Q & 0x07) << 5) + ( (P & 0x07) << 2 )+ (f_range & 0x03);
		CDCE813Q1_WriteOneByte(0x18, reg18);
		CDCE813Q1_WriteOneByte(0x19, reg19);
		CDCE813Q1_WriteOneByte(0x1A, reg1A);
		CDCE813Q1_WriteOneByte(0x1B, reg1B);
		CDCE813Q1_WriteOneByte(0x1C, N);
		CDCE813Q1_WriteOneByte(0x1D, ((N & 0xf) << 4) + (R & 0xf0));
		CDCE813Q1_WriteOneByte(0x1E, (R & 0x0f) + (Q & 0xf0));
		CDCE813Q1_WriteOneByte(0x1F, ((Q & 0x07) << 5) + ((P & 0x07) << 2) + (f_range & 0x03));
		S0 = 1;
	}
	else//���Ƶ�ʹ��ͣ����Y1�ڣ�������·PLL���
	{
		Pdiv = Pdiv/10;
		CDCE813Q1_Init();				    //��ʼ��IIC 
		delay_ms(10);
		// �趨Y1��pdiv1��Ԥ��Ƶֵ
		CDCE813Q1_WriteOneByte(0x02, 0x34+ (  (Pdiv>>8)&0x3 ) );
		CDCE813Q1_WriteOneByte(0x03, (u8)Pdiv);
		//д��ͨ�üĴ���
		CDCE813Q1_WriteOneByte(0x04, 0x02);
		CDCE813Q1_WriteOneByte(0x05, 0x00);
		CDCE813Q1_WriteOneByte(0x06, 0x40);
		CDCE813Q1_WriteOneByte(0x12, 0x00);
		CDCE813Q1_WriteOneByte(0x13, 0x01);
		CDCE813Q1_WriteOneByte(0x14, 0xED);
		CDCE813Q1_WriteOneByte(0x15, 0x02);
		//д��N,P,R,Q,�Լ�f_range��ֵ
		reg18 = (N >> 4) & 0xFF;
		reg19 = ( (N & 0x00f) << 4 )+ ((R & 0xf0) >> 5);
		reg1A = ((R & 0x1f) << 3 )+ ((Q >> 3) & 0x07);
		reg1B = ((Q & 0x07) << 5) + ( (P & 0x07) << 2 )+ (f_range & 0x03);
		CDCE813Q1_WriteOneByte(0x18, reg18);
		CDCE813Q1_WriteOneByte(0x19, reg19);
		CDCE813Q1_WriteOneByte(0x1A, reg1A);
		CDCE813Q1_WriteOneByte(0x1B, reg1B);
		CDCE813Q1_WriteOneByte(0x1C, N);
		CDCE813Q1_WriteOneByte(0x1D, ((N & 0xf) << 4) + (R & 0xf0));
		CDCE813Q1_WriteOneByte(0x1E, (R & 0x0f) + (Q & 0xf0));
		CDCE813Q1_WriteOneByte(0x1F, ((Q & 0x07) << 5) + ((P & 0x07) << 2) + (f_range & 0x03));
		S0 = 1;
	}
}

void Setup_Y2Hz(u32 hz)//�����޸�Y2���Ƶ��
{
	u16 i = 1;
	u32 M, N, Pdiv, Q, R;
	u8 reg18, reg19, reg1A, reg1B;
	int P;
	u32 f_vco = hz;
  int result = 0;
	u8 f_range;

	while (f_vco < 800000 )
	{
		i++;		
		f_vco = hz * i;
	}

	while (f_vco <= 2300000 )
	{
		for (N = 4095; N > 0; N--)
		{
			for (M = 511; M > 0; M--)
			{
				if ((N * CLK_IN / M) == f_vco)
				{ 
						result = 1;
						break;				
				}
			}
			if (result)
			{
				break;
			}
		}
		if (result)
		{
			break;
		}
		else
		{
			i++;
			f_vco = hz * i;
		}
	}

	if (!result)
	{
		return;
	}
	
	P = 4 - (int)((log((double)N / (double)M))/log(2));//����P
	if (P < 0)
	{
		P = 0;
	}
	Q = (int)((double)N * pow(2, (double)P) / (double)M );//����Q
	R = (double)N * pow(2, (double)P) - M * Q;//����R
	if (f_vco < 1250000 )
	{
		f_range = 0;
	}
	else if ((f_vco >= 1250000 ) && (f_vco < 1500000 ))
	{
		f_range = 1;
	}
	else if ((f_vco >= 1500000 ) && (f_vco < 1750000 ))
	{
		f_range = 2;
	}
	else
	{
		f_range = 3;
	}
	S0 = 0;
	Pdiv = f_vco / hz;
	printf("Fvco = %d .... Pdiv = %d  ...\r\n" ,f_vco,Pdiv) ; 
	printf("M = %d...N = %d....P = %d....R = %d...Q = %d...Pdiv = %d\r\n",M , N,P , R , Q , Pdiv);
		CDCE813Q1_Init();				    //��ʼ��IIC 
		delay_ms(10);
		//д��ͨ�üĴ���
		CDCE813Q1_WriteOneByte(0x04, 0x02);
		CDCE813Q1_WriteOneByte(0x05, 0x00);
		CDCE813Q1_WriteOneByte(0x06, 0x40);
		CDCE813Q1_WriteOneByte(0x12, 0x00);
		CDCE813Q1_WriteOneByte(0x13, 0x01);
		CDCE813Q1_WriteOneByte(0x14, 0x6D);
		CDCE813Q1_WriteOneByte(0x15, 0x02);
		// �趨Y2��pdiv1��Ԥ��Ƶֵ	
		CDCE813Q1_WriteOneByte(0x16,  0X7f&(Pdiv) );
		//д��N,P,R,Q,�Լ�f_range��ֵ		
		reg18 = (N >> 4) & 0xFF;
		reg19 = ( (N & 0x00f) << 4 )+ ((R & 0xf0) >> 5);
		reg1A = ((R & 0x1f) << 3 )+ ((Q >> 3) & 0x07);
		reg1B = ((Q & 0x07) << 5) + ( (P & 0x07) << 2 )+ (f_range & 0x03);
		CDCE813Q1_WriteOneByte(0x18, reg18);
		CDCE813Q1_WriteOneByte(0x19, reg19);
		CDCE813Q1_WriteOneByte(0x1A, reg1A);
		CDCE813Q1_WriteOneByte(0x1B, reg1B);
		CDCE813Q1_WriteOneByte(0x1C, N);
		CDCE813Q1_WriteOneByte(0x1D, ((N & 0xf) << 4) + (R & 0xf0));
		CDCE813Q1_WriteOneByte(0x1E, (R & 0x0f) + (Q & 0xf0));
		CDCE813Q1_WriteOneByte(0x1F, ((Q & 0x07) << 5) + ((P & 0x07) << 2) + (f_range & 0x03));
		S0 = 1;
}

void Setup_Y3Hz(u32 hz)//�����޸�Y3���Ƶ��
{
	u16 i = 1;
	u32 M, N, Pdiv, Q, R;
	u8 reg18, reg19, reg1A, reg1B;
	int P;
	u32 f_vco = hz;
  int result = 0;
	u8 f_range;

	while (f_vco < 800000 )
	{
		i++;		
		f_vco = hz * i;
	}

	while (f_vco <= 2300000 )
	{
		for (N = 4095; N > 0; N--)
		{
			for (M = 511; M > 0; M--)
			{
				if ((N * CLK_IN / M) == f_vco)
				{ 
						result = 1;
						break;				
				}
			}
			if (result)
			{
				break;
			}
		}
		if (result)
		{
			break;
		}
		else
		{
			i++;
			f_vco = hz * i;
		}
	}

	if (!result)
	{
		return;
	}
	
	P = 4 - (int)((log((double)N / (double)M))/log(2));//����P
	if (P < 0)
	{
		P = 0;
	}
	Q = (int)((double)N * pow(2, (double)P) / (double)M );//����Q
	R = (double)N * pow(2, (double)P) - M * Q;//����R
	if (f_vco < 1250000 )
	{
		f_range = 0;
	}
	else if ((f_vco >= 1250000 ) && (f_vco < 1500000 ))
	{
		f_range = 1;
	}
	else if ((f_vco >= 1500000 ) && (f_vco < 1750000 ))
	{
		f_range = 2;
	}
	else
	{
		f_range = 3;
	}
	S0 = 0;
	Pdiv = f_vco / hz;
	printf("Fvco = %d .... Pdiv = %d  ...\r\n" ,f_vco,Pdiv) ; 
	printf("M = %d...N = %d....P = %d....R = %d...Q = %d...Pdiv = %d\r\n",M , N,P , R , Q , Pdiv);
		CDCE813Q1_Init();				    //��ʼ��IIC 
		delay_ms(10);
		//д��ͨ�üĴ���
		CDCE813Q1_WriteOneByte(0x04, 0x02);
		CDCE813Q1_WriteOneByte(0x05, 0x00);
		CDCE813Q1_WriteOneByte(0x06, 0x40);
		CDCE813Q1_WriteOneByte(0x12, 0x00);
		CDCE813Q1_WriteOneByte(0x13, 0x01);
		CDCE813Q1_WriteOneByte(0x14, 0x6D);
		CDCE813Q1_WriteOneByte(0x15, 0x02);
		// �趨Y3��pdiv1��Ԥ��Ƶֵ
		CDCE813Q1_WriteOneByte(0x17,  0X7f&(Pdiv) );
		//д��N,P,R,Q,�Լ�f_range��ֵ		
		reg18 = (N >> 4) & 0xFF;
		reg19 = ( (N & 0x00f) << 4 )+ ((R & 0xf0) >> 5);
		reg1A = ((R & 0x1f) << 3 )+ ((Q >> 3) & 0x07);
		reg1B = ((Q & 0x07) << 5) + ( (P & 0x07) << 2 )+ (f_range & 0x03);
		CDCE813Q1_WriteOneByte(0x18, reg18);
		CDCE813Q1_WriteOneByte(0x19, reg19);
		CDCE813Q1_WriteOneByte(0x1A, reg1A);
		CDCE813Q1_WriteOneByte(0x1B, reg1B);
		CDCE813Q1_WriteOneByte(0x1C, N);
		CDCE813Q1_WriteOneByte(0x1D, ((N & 0xf) << 4) + (R & 0xf0));
		CDCE813Q1_WriteOneByte(0x1E, (R & 0x0f) + (Q & 0xf0));
		CDCE813Q1_WriteOneByte(0x1F, ((Q & 0x07) << 5) + ((P & 0x07) << 2) + (f_range & 0x03));
		S0 = 1;
}
