#include "bsp_key.h"  


//������־λ�����а�������ʱ����Ӧ��־λΪ1����main��������ú�Ҫ����
uint8_t key1_val,key2_val,key3_val,key4_val;

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* �����ж�Դ������ */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = KEY2_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = KEY3_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = KEY4_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//��������ʱ��IO�ڵ͵�ƽ����ʹ����������
/**
  * @brief  ���ð����õ���I/O�� 
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);
	NVIC_Configuration();
	
/*--------------------------KEY1����-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(KEY1_EXTI_PORTSOURCE, KEY1_EXTI_PINSOURCE); 
	EXTI_InitStructure.EXTI_Line = KEY1_EXTI_LINE;

	/* EXTIΪ�ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* ʹ���ж� */	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

/*--------------------------KEY2����-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	

	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(KEY2_EXTI_PORTSOURCE, KEY2_EXTI_PINSOURCE); 
	EXTI_InitStructure.EXTI_Line = KEY2_EXTI_LINE;

	/* EXTIΪ�ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* ʹ���ж� */	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


/*--------------------------KEY3����-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	

	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(KEY3_EXTI_PORTSOURCE, KEY3_EXTI_PINSOURCE); 
	EXTI_InitStructure.EXTI_Line = KEY3_EXTI_LINE;

	/* EXTIΪ�ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* ʹ���ж� */	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	/*--------------------------KEY4����-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	

	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(KEY4_EXTI_PORTSOURCE, KEY4_EXTI_PINSOURCE); 
	EXTI_InitStructure.EXTI_Line = KEY4_EXTI_LINE;

	/* EXTIΪ�ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* ʹ���ж� */	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void KEY1_IRQHandler(void)
{

  //ȷ���Ƿ������KEY1_EXTI_LINE�ж�
	if(EXTI_GetITStatus(KEY1_EXTI_LINE) != RESET) 
	{ 
		key1_val = 1;
         //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_EXTI_LINE);     
	}  
}

void KEY2_IRQHandler(void)
{

  //ȷ���Ƿ������KEY2_EXTI_LINE�ж�
	if(EXTI_GetITStatus(KEY2_EXTI_LINE) != RESET) 
	{ 
		key2_val = 1;
         //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY2_EXTI_LINE);     
	}  	
}


void KEY3_IRQHandler(void)
{

  //ȷ���Ƿ������KEY3_EXTI_LINE�ж�
	if(EXTI_GetITStatus(KEY3_EXTI_LINE) != RESET) 
	{ 
		key3_val = 1;
         //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY3_EXTI_LINE);     
	}  	
}

void KEY4_IRQHandler(void)
{

  //ȷ���Ƿ������KEY4_EXTI_LINE�ж�
	if(EXTI_GetITStatus(KEY4_EXTI_LINE) != RESET) 
	{ 
		key4_val = 1;
         //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY4_EXTI_LINE);     
	}  	
}

