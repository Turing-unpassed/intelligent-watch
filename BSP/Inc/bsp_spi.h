#ifndef BSP_SPI_H
#define BSP_SPI_H

#ifdef __cplusplus
extern "C"
{
#endif

/*在此处引用外部文件：       begin*/	
#include "spi.h"
#include "RTT_Wrapper.h"
/*引用外部文件end*/	

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define MAX_SPI_BUS_NUM 2

typedef enum{
    SPI_MODE_NONE=0,
    SPI_BLOCKING,
    SPI_IT,
    SPI_DMA
}SPI_Mode;

class SPI_Base
{
private:
    SPI_Mode mode;
    uint16_t timeout;

public:
    SPI_HandleTypeDef* hspi;
    static SPI_Base* SPI_BUS[MAX_SPI_BUS_NUM];
    static uint8_t SPI_BUS_Index;

    SPI_Base(SPI_HandleTypeDef* hspi, SPI_Mode mode, uint16_t timeout=500);
    virtual void SPI_Send(uint8_t* pData, uint16_t Size);
    virtual void SPI_Receive(uint8_t* pData, uint16_t Size);
    virtual void SPI_Rx_Callback()=0;
};

#endif

#endif