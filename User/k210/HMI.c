#include "HMI.h"
#include "stm32f10x.h"
#include "math.h"
#include "stdio.h"
void HMISends(char *buf1)		  //�ַ������ͺ���
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(UART4,buf1[i]);  //����һ���ֽ�
			while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		 	i++;
		}
	 else 
	 return ;

		}
	}
void HMISendb(int k)		         //�ֽڷ��ͺ���
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(UART4,k);  //����һ���ֽ�
			while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
	 else 
	 return ;

	 } 
}

