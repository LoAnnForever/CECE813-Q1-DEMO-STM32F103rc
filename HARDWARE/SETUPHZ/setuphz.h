#ifndef __SETUPHZ_H
#define __SETUPHZ_H

#include "sys.h"

void Setup_YHz(u32 hz , u32 Yout2 , u32 Yout3) ; 
void Setup_Y2Hz(u32 hz);
void Setup_Y3Hz(u32 hz);
void Setup_Y1Hz(u32 hz);

extern u8 Open_CloY23 ;
#endif
