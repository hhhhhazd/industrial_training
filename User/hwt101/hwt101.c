#include "hwt101.h"
float yaw = 0;
// jy61p参数定义
#define ACC_UPDATE   0x01
#define GYRO_UPDATE  0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE   0x08
#define READ_UPDATE  0x80
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
const uint32_t c_uiBaud[10] = {0, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
uint8_t array_yaw[20]       = {0};
static void CmdProcess(void);
void jy61p_init();
static void AutoScanSensor(void);
static void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static void Delayms(uint16_t ucMs);
void Usart2Init(unsigned int uiBaud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 485 control pin
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = uiBaud;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 8;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
double custom_fmod(double a, double b)
{
    return a - b * floor(a / b);
}

double convert_yaw(double yaw)
{
    return custom_fmod(yaw + 180.0, 360.0) - 180.0;
}
void CopeSerial2Data(unsigned char ucData)
{
    static unsigned char ucRxBuffer[250];
    static unsigned char ucRxCnt = 0;

    ucRxBuffer[ucRxCnt++] = ucData;
    if (ucRxBuffer[0] != 0x55) // 校验数据头
    {
        ucRxCnt = 0;
        return;
    }

    if (ucRxCnt < 11) { return; } // 判断一帧数据是否接收完成
    {
        switch (ucRxBuffer[1]) {
            case 0x53:
                yaw = (((short)ucRxBuffer[7] << 8) | ucRxBuffer[6]) / 32768.0 * 180.0;
                //把0到360转化为-180到180
                yaw = convert_yaw(yaw);
                break;
        }
        ucRxCnt = 0;
    }
}
void Uartx_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num)
{
    uint8_t i;

    for (i = 0; i < num; i++) {
        /* 发送一个字节数据到USART */
        Usart_SendByte(pUSARTx, array[i]);
    }
    /* 等待发送完成 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

void Yaw_setzero()
{
    array_yaw[0] = 0xFF;
    array_yaw[1] = 0xAA;
    array_yaw[2] = 0x76;
    array_yaw[3] = 0x00;
    array_yaw[4] = 0x00;

    Uartx_SendArray(USART2, array_yaw, 5);
}

void USART2_IRQHandler(void)
{
    unsigned char ucTemp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        CopeSerial2Data((unsigned char)USART2->DR); // 接收数据
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
void Uart2Send(unsigned char *p_data, unsigned int uiSize)
{
    unsigned int i;
    for (i = 0; i < uiSize; i++) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *p_data++);
    }
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
void jy61p_init()
{
    
    WitInit(WIT_PROTOCOL_NORMAL, 0x50);
    WitSerialWriteRegister(SensorUartSend);
    WitRegisterCallBack(SensorDataUpdata);
    WitDelayMsRegister(Delayms);
    AutoScanSensor();
}

void CopeCmdData(unsigned char ucData)
{
    static unsigned char s_ucData[50], s_ucRxCnt = 0;

    s_ucData[s_ucRxCnt++] = ucData;
    if (s_ucRxCnt < 3) return; // Less than three data returned
    if (s_ucRxCnt >= 50) s_ucRxCnt = 0;
    if (s_ucRxCnt >= 3) {
        if ((s_ucData[1] == '\r') && (s_ucData[2] == '\n')) {
            s_cCmd = s_ucData[0];
            memset(s_ucData, 0, 50); //
            s_ucRxCnt = 0;
        } else {
            s_ucData[0] = s_ucData[1];
            s_ucData[1] = s_ucData[2];
            s_ucRxCnt   = 2;
        }
    }
}
static void ShowHelp(void)
{
    printf("\r\n************************	 WIT_SDK_DEMO	************************");
    printf("\r\n************************          HELP           ************************\r\n");
    printf("UART SEND:a\\r\\n   Acceleration calibration.\r\n");
    printf("UART SEND:m\\r\\n   Magnetic field calibration,After calibration send:   e\\r\\n   to indicate the end\r\n");
    printf("UART SEND:U\\r\\n   Bandwidth increase.\r\n");
    printf("UART SEND:u\\r\\n   Bandwidth reduction.\r\n");
    printf("UART SEND:B\\r\\n   Baud rate increased to 115200.\r\n");
    printf("UART SEND:b\\r\\n   Baud rate reduction to 9600.\r\n");
    printf("UART SEND:R\\r\\n   The return rate increases to 10Hz.\r\n");
    printf("UART SEND:r\\r\\n   The return rate reduction to 1Hz.\r\n");
    printf("UART SEND:C\\r\\n   Basic return content: acceleration, angular velocity, angle, magnetic field.\r\n");
    printf("UART SEND:c\\r\\n   Return content: acceleration.\r\n");
    printf("UART SEND:h\\r\\n   help.\r\n");
    printf("******************************************************************************\r\n");
}

static void CmdProcess(void)
{
    switch (s_cCmd) {
        case 'a':
            if (WitStartAccCali() != WIT_HAL_OK)
                printf("\r\nSet AccCali Error\r\n");
            break;
        case 'm':
            if (WitStartMagCali() != WIT_HAL_OK)
                printf("\r\nSet MagCali Error\r\n");
            break;
        case 'e':
            if (WitStopMagCali() != WIT_HAL_OK)
                printf("\r\nSet MagCali Error\r\n");
            break;
        case 'u':
            if (WitSetBandwidth(BANDWIDTH_5HZ) != WIT_HAL_OK)
                printf("\r\nSet Bandwidth Error\r\n");
            break;
        case 'U':
            if (WitSetBandwidth(BANDWIDTH_256HZ) != WIT_HAL_OK)
                printf("\r\nSet Bandwidth Error\r\n");
            break;
        case 'B':
            if (WitSetUartBaud(WIT_BAUD_115200) != WIT_HAL_OK)
                printf("\r\nSet Baud Error\r\n");
            else
                Usart2Init(c_uiBaud[WIT_BAUD_115200]);
            break;
        case 'b':
            if (WitSetUartBaud(WIT_BAUD_9600) != WIT_HAL_OK)
                printf("\r\nSet Baud Error\r\n");
            else
                Usart2Init(c_uiBaud[WIT_BAUD_9600]);
            break;
        case 'R':
            if (WitSetOutputRate(RRATE_10HZ) != WIT_HAL_OK)
                printf("\r\nSet Rate Error\r\n");
            break;
        case 'r':
            if (WitSetOutputRate(RRATE_1HZ) != WIT_HAL_OK)
                printf("\r\nSet Rate Error\r\n");
            break;
        case 'C':
            if (WitSetContent(RSW_ACC | RSW_GYRO | RSW_ANGLE | RSW_MAG) != WIT_HAL_OK)
                printf("\r\nSet RSW Error\r\n");
            break;
        case 'c':
            if (WitSetContent(RSW_ACC) != WIT_HAL_OK)
                printf("\r\nSet RSW Error\r\n");
            break;
        case 'h':
            ShowHelp();
            break;
    }
    s_cCmd = 0xff;
}

static void SensorUartSend(uint8_t *p_data, uint32_t uiSize)
{
    Uart2Send(p_data, uiSize);
}

static void Delayms(uint16_t ucMs)
{
    delay_ms(ucMs);
}

static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
    int i;
    for (i = 0; i < uiRegNum; i++) {
        switch (uiReg) {
                //            case AX:
                //            case AY:
            case AZ:
                s_cDataUpdate |= ACC_UPDATE;
                break;
                //            case GX:
                //            case GY:
            case GZ:
                s_cDataUpdate |= GYRO_UPDATE;
                break;
                //            case HX:
                //            case HY:
            case HZ:
                s_cDataUpdate |= MAG_UPDATE;
                break;
                //            case Roll:
                //            case Pitch:
            case Yaw:
                s_cDataUpdate |= ANGLE_UPDATE;
                break;
            default:
                s_cDataUpdate |= READ_UPDATE;
                break;
        }
        uiReg++;
    }
}

static void AutoScanSensor(void)
{
    int i, iRetry;

    for (i = 1; i < 10; i++) {
        Usart2Init(c_uiBaud[i]);
        iRetry = 2;
        do {
            s_cDataUpdate = 0;
            WitReadReg(AX, 3);
            delay_ms(100);
            if (s_cDataUpdate != 0) {
                printf("%d baud find sensor\r\n\r\n", c_uiBaud[i]);
                ShowHelp();
                return;
            }
            iRetry--;
        } while (iRetry);
    }
    printf("can not find sensor\r\n");
    printf("please check your connection\r\n");
}
