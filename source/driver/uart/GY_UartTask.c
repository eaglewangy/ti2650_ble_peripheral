#include "board.h"
#include <ti/drivers/uart/UARTCC26XX.h>

#include "stdio.h"
#include "string.h"
#include "GY_UartTask.h"
#include "simple_peripheral.h"

/*********************************************************************
 * LOCAL PARAMETER
 */
UART_Handle UARTHandle;
UART_Params UARTparams;
uint8_t Uart_RxTempBuf[200];

GY_UartRxBufCallback GY_UartReviceDataCallback;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
void Uart_WriteCallback(UART_Handle handle, void *txBuf, size_t size);
void Uart_ReadCallback(UART_Handle handle, void *rxBuf, size_t size);

/*********************************************************************
 * @fn      Uart_ReadCallback
 *
 * @brief   ���ڶ��ص�
 *
 * @param   handle -> ����ͨ��
 *          rxBuf -> ���ڽ������ݵ�ָ��
 *          size -> ���ڽ������ݵĳ���
 *
 * @return  None.
 */
void Uart_ReadCallback(UART_Handle handle, void *rxBuf, size_t size)
{ 
  //UART_write(UARTHandle, rxBuf, size);       //���Դ�ӡ
  UART_read(handle, Uart_RxTempBuf, 200);      //�ٴδ�һ�����ڶ�
  GY_UartReviceDataCallback(rxBuf, size);      //��app����һ�����ڶ��ص�
}

/*********************************************************************
 * @fn      Uart_WriteCallback
 *
 * @brief   ����д�ص�
 *
 * @param   handle -> ����ͨ��
 *          txBuf -> ���ڷ������ݵ�ָ��
 *          size -> ���ڷ������ݵĳ���
 *
 * @return  None.
 */
void Uart_WriteCallback(UART_Handle handle, void *txBuf, size_t size)
{
  //
}

/*********************************************************************
 * @fn      GY_UartTask_Init
 *
 * @brief   ���������ʼ��
 *
 * @param   None
 *
 * @return  None.
 */
void GY_UartTask_Init(void)
{
  UART_init();                                      //��ʼ��ģ��Ĵ��ڹ���
  UART_Params_init(&UARTparams);                    //��ʼ�����ڲ���
  UARTparams.baudRate = 115200;                     //���ڲ�����115200
  UARTparams.dataLength = UART_LEN_8;               //��������λ8
  UARTparams.stopBits = UART_STOP_ONE;              //����ֹͣλ1
  UARTparams.readDataMode = UART_DATA_BINARY;       //���ڽ������ݲ�������
  UARTparams.writeDataMode = UART_DATA_BINARY;      //���ڷ������ݲ�������
  UARTparams.readMode = UART_MODE_CALLBACK;         //�����첽��
  UARTparams.writeMode = UART_MODE_CALLBACK;        //�����첽д
  UARTparams.readEcho = UART_ECHO_OFF;              //���ڲ�����
  UARTparams.readReturnMode = UART_RETURN_NEWLINE;  //�����յ����з�ʱ���ص�
  UARTparams.readCallback = Uart_ReadCallback;      //���ڶ��ص�
  UARTparams.writeCallback = Uart_WriteCallback;    //����д�ص�
  
  UARTHandle = UART_open(Board_UART0, &UARTparams); //�򿪴���ͨ��
  UART_control(UARTHandle, UARTCC26XX_RETURN_PARTIAL_ENABLE,  NULL);   //������ղ��ֻص�
  
  UART_read(UARTHandle, Uart_RxTempBuf, 200);       //��һ�����ڶ�
}

/*********************************************************************
 * @fn      GY_UartTask_Write
 *
 * @brief   ����д����
 *
 * @param   buf -> ��Ҫд������ָ��
 *          len -> ��Ҫд�����ݳ���
 *
 * @return  None.
 */
void GY_UartTask_Write(uint8_t *buf, uint16_t len)
{
  UART_write(UARTHandle, buf, len);
}

/*********************************************************************
 * @fn      GY_UartTask_Printf
 *
 * @brief   ����д����������ϵͳprintf��
 *
 * @param   format -> ����������־λ������%d,%s��
 *          ... -> ��������
 *
 * @return  None.
 */
void GY_UartTask_Printf(const char* format, ...)
{
  va_list arg;
  va_start(arg,format);
  uint8_t buf[200];
  uint16_t len;
  len = vsprintf((char*)buf, format, arg);
  UART_write(UARTHandle, buf, len);
}
/*********************************************************************
 * @fn      GY_UartTask_RegisterPacketReceivedCallback
 *
 * @brief   ע�ᴮ�ڽ��ջص����񣨽����ڽ��յ����ݴ���app����ȥ����
 *
 * @param   callback -> ���ڽ������ݻص�����������buf��len��
 *
 * @return  None.
 */
void GY_UartTask_RegisterPacketReceivedCallback(GY_UartRxBufCallback callback)
{
  GY_UartReviceDataCallback = callback;
}