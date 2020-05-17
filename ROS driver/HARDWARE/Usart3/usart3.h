#ifndef __USRAT3_H
#define __USRAT3_H 

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"	  	

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;  
	
extern u8 Usart3_Receive;
void uart3_init(u32 pclk2,u32 bound);
	
#endif
#ifdef __cplusplus
}
#endif
