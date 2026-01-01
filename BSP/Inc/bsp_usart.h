#ifndef BSP_USART_H
#define BSP_USART_H

#ifdef __cplusplus
extern "C"
{
#endif

/*在此处引用外部文件：       begin*/	
#include "usart.h"
#include "RTT_Wrapper.h"
/*引用外部文件end*/	

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define MAX_UART_NUM 3

typedef enum{
    UART_MODE_NONE=0,
    UART_BLOCKING,
    UART_IT,
    UART_DMA,
    UART_IDLE_BLOCKING,
    UART_IDLE_IT,
    UART_IDLE_DMA
}UART_Mode;

class UART_Base
{
private:
    UART_Mode tx_mode,rx_mode;
    uint16_t idle_rx_len;
    uint16_t timeout;
public:
    UART_HandleTypeDef* huart;
    static UART_Base* UART_INSTANCES[MAX_UART_NUM];
    static uint8_t UART_INSTANCES_Index;
    
    UART_Base(UART_HandleTypeDef* huart,UART_Mode tx_mode,UART_Mode rx_mode,uint16_t timeout);
    virtual void UART_Send(uint8_t* pData, uint16_t Size);
    virtual void UART_Receive(uint8_t* pData, uint16_t Size);
    virtual void UART_Rx_Callback();
};

#endif

#endif