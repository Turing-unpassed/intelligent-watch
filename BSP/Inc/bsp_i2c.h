#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C"
{
#endif

/*在此处引用外部文件：       begin*/	
#include "i2c.h"
#include "RTT_Wrapper.h"
/*引用外部文件end*/	

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define MAX_I2C_BUS_NUM 2

typedef enum{
    I2C_MASTER=0,
    I2C_SLAVE,
    I2C_MASTER_MEM
}I2C_ROLE;

typedef enum{   
    I2C_MODE_NONE=0,
    I2C_BLOCKING,
    I2C_IT,
    I2C_DMA
}I2C_Mode;

class I2C_Base
{
private:
    I2C_ROLE role;
    I2C_Mode mode;

    uint16_t timeout;
    uint16_t DevAddress,MemAddress,MemAddSize;
    
public:
    I2C_HandleTypeDef* hi2c;
    static I2C_Base* I2C_BUS[MAX_I2C_BUS_NUM];
    static uint8_t I2C_BUS_Index;

    I2C_Base(I2C_HandleTypeDef* hi2c,I2C_ROLE role,I2C_Mode mode,uint16_t DevAddress,uint16_t MemAddress,uint16_t MemAddSize,uint16_t timeout=500);
    virtual void I2C_Send(uint8_t* pData, uint16_t Size);
    virtual void I2C_Receive(uint8_t* pData, uint16_t Size);
    virtual void I2C_Rx_Callback()=0;
};

#endif

#endif