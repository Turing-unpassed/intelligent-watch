#include "bsp_i2c.h"

I2C_Base* I2C_Base::I2C_BUS[MAX_I2C_BUS_NUM]={nullptr};
uint8_t I2C_Base::I2C_BUS_Index=0;

I2C_Base::I2C_Base(I2C_HandleTypeDef* hi2c,I2C_ROLE role,I2C_Mode mode,uint16_t DevAddress,uint16_t MemAddress,uint16_t MemAddSize,uint16_t timeout)
{
    this->role=role;
    this->mode=mode;
    this->DevAddress=DevAddress;
    this->MemAddress=MemAddress;
    this->MemAddSize=MemAddSize;
    this->timeout=timeout;
    this->hi2c=hi2c;
    if(I2C_BUS_Index<MAX_I2C_BUS_NUM)
    {
        I2C_BUS[I2C_BUS_Index]=this;
        I2C_BUS_Index++;
    }
}

void I2C_Base::I2C_Send(uint8_t* pData, uint16_t Size)
{
    switch(mode){
        case I2C_BLOCKING:
            if(role==I2C_MASTER){
                if(HAL_I2C_Master_Transmit(hi2c,DevAddress,pData,Size,timeout)!=HAL_OK){
                    LOG_ERROR("I2C Master Transmit Error");
                }
            }else if(role==I2C_SLAVE){
                if(HAL_I2C_Slave_Transmit(hi2c,pData,Size,timeout)!=HAL_OK){
                    LOG_ERROR("I2C Slave Transmit Error");
                }
            }else{
                if(HAL_I2C_Mem_Write(hi2c,DevAddress,MemAddress, MemAddSize ,pData,Size,timeout)!=HAL_OK){
                    LOG_ERROR("I2C Memory Write Error");
                }
            }
            break;
        case I2C_IT:
            if(role==I2C_MASTER){
                if(HAL_I2C_Master_Transmit_IT(hi2c,DevAddress,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Master Transmit IT Error");
                }
            }else if(role==I2C_SLAVE){
                if(HAL_I2C_Slave_Transmit_IT(hi2c,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Slave Transmit IT Error");
                }
            }else{
                if(HAL_I2C_Mem_Write_IT(hi2c,DevAddress,MemAddress, MemAddSize ,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Memory Write IT Error");
                }
            }
            break;
        case I2C_DMA:
            if(role==I2C_MASTER){
                if(HAL_I2C_Master_Transmit_DMA(hi2c,DevAddress,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Master Transmit DMA Error");
                }
            }else if(role==I2C_SLAVE){
                if(HAL_I2C_Slave_Transmit_DMA(hi2c,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Slave Transmit DMA Error");
                }
            }else{
                if(HAL_I2C_Mem_Write_DMA(hi2c,DevAddress,MemAddress, MemAddSize ,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Memory Write DMA Error");
                }
            }
            break;
        default:
            break;
    }
}

void I2C_Base::I2C_Receive(uint8_t* pData, uint16_t Size)
{
    switch(mode){
        case I2C_BLOCKING:
            if(role==I2C_MASTER){
                if(HAL_I2C_Master_Receive(hi2c,DevAddress,pData,Size,timeout)!=HAL_OK){
                    LOG_ERROR("I2C Master Receive Error");
                }
            }else if(role==I2C_SLAVE){
                if(HAL_I2C_Slave_Receive(hi2c,pData,Size,timeout)!=HAL_OK){
                    LOG_ERROR("I2C Slave Receive Error");
                }
            }else{
                if(HAL_I2C_Mem_Read(hi2c,DevAddress,MemAddress, MemAddSize ,pData,Size,timeout)!=HAL_OK){
                    LOG_ERROR("I2C Memory Read Error");
                }
            }
            break;
        case I2C_IT:
            if(role==I2C_MASTER){
                if(HAL_I2C_Master_Receive_IT(hi2c,DevAddress,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Master Receive IT Error");
                }
            }else if(role==I2C_SLAVE){
                if(HAL_I2C_Slave_Receive_IT(hi2c,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Slave Receive IT Error");
                }
            }else{
                if(HAL_I2C_Mem_Read_IT(hi2c,DevAddress,MemAddress, MemAddSize ,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Memory Read IT Error");
                }
            }
            break;
        case I2C_DMA:
            if(role==I2C_MASTER){
                if(HAL_I2C_Master_Receive_DMA(hi2c,DevAddress,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Master Receive DMA Error");
                }
            }else if(role==I2C_SLAVE){
                if(HAL_I2C_Slave_Receive_DMA(hi2c,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Slave Receive DMA Error");
                }
            }else{
                if(HAL_I2C_Mem_Read_DMA(hi2c,DevAddress,MemAddress, MemAddSize ,pData,Size)!=HAL_OK){
                    LOG_ERROR("I2C Memory Read DMA Error");
                }
            }
            break;
        default:
            break;
    }
}

void I2C_Base::I2C_Rx_Callback()
{

}

extern "C" void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
    for(uint8_t i=0;i<I2C_Base::I2C_BUS_Index;i++){
        if(I2C_Base::I2C_BUS[i]->hi2c==hi2c){
            I2C_Base::I2C_BUS[i]->I2C_Rx_Callback();
            break;
        }
    }
}

extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
    for(uint8_t i=0;i<I2C_Base::I2C_BUS_Index;i++){
        if(I2C_Base::I2C_BUS[i]->hi2c==hi2c){
            I2C_Base::I2C_BUS[i]->I2C_Rx_Callback();
            break;
        }
    }
}
                