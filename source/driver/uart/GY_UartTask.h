#ifndef SERIAL_UART_H
#define SERIAL_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************
 * 串口任务初始化
*/  
void GY_UartTask_Init(void);

/*****************************************************
 * 串口写函数
*/
void GY_UartTask_Write(uint8_t *buf, uint16_t len);  //打印字符串
void GY_UartTask_Printf(const char* format, ...);   //printf打印
/*****************************************************
 * 串口接收数据回调（包括数据buf及len）
*/
typedef void (*GY_UartRxBufCallback)(uint8_t *buf, uint16_t len);

/*****************************************************
 * 注册串口接收回调任务（将串口接收的数据传给app任务去处理）
*/
void GY_UartTask_RegisterPacketReceivedCallback(GY_UartRxBufCallback callback);


#ifdef __cplusplus
{
#endif // extern "C"

#endif // end of SERIAL_UART_H definition