#include "bsp_usart.h"

UART_Base* UART_Base::UART_INSTANCES[MAX_UART_NUM]={nullptr};
uint8_t UART_Base::UART_INSTANCES_Index=0;

UART_Base::UART_Base(UART_HandleTypeDef* huart,UART_Mode tx_mode,UART_Mode rx_mode,uint16_t timeout)
{
    this->huart=huart;
    this->tx_mode=tx_mode;
    this->rx_mode=rx_mode;
    this->timeout=timeout;
    if(UART_INSTANCES_Index<MAX_UART_NUM)
    {
        UART_INSTANCES[UART_INSTANCES_Index++]=this;
    }else
    {
        LOG_WARNING("UART Instances exceed MAX_UART_NUM");
    }
    
}

void UART_Base::UART_Send(uint8_t* pData, uint16_t Size)
{
    HAL_StatusTypeDef ret;
    switch(tx_mode)
    {
        case UART_BLOCKING:
            ret = HAL_UART_Transmit(huart,pData,Size,timeout);
            if(ret == HAL_OK){
                LOG_INFO("UART send success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART send error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART send busy");
            }else
            {
                LOG_WARNING("UART send timeout");
            }
            break;
        case UART_IT:
            ret = HAL_UART_Transmit_IT(huart,pData,Size);
            if(ret == HAL_OK){
                LOG_INFO("UART send success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART send error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART send busy");
            }
            break;
        case UART_DMA:
            ret = HAL_UART_Transmit_DMA(huart,pData,Size);
            if(ret == HAL_OK){
                LOG_INFO("UART send success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART send error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART send busy");
            }
            break;
        default:
            break;
    }

}

void UART_Base::UART_Receive(uint8_t* pData, uint16_t Size)
{
    HAL_StatusTypeDef ret;
    switch(rx_mode)
    {   
        case UART_BLOCKING:
            ret = HAL_UART_Receive(huart,pData,Size,timeout);
            if(ret == HAL_OK){
                LOG_INFO("UART receive success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART receive error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART receive busy");
            }else
            {
                LOG_WARNING("UART receive timeout");
            }
            break;
        case UART_IT:
            ret = HAL_UART_Receive_IT(huart,pData,Size);
            if(ret == HAL_OK){
                LOG_INFO("UART receive success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART receive error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART receive busy");
            }
            break;
        case UART_DMA:
            ret = HAL_UART_Receive_DMA(huart,pData,Size);
            if(ret == HAL_OK){
                LOG_INFO("UART receive success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART receive error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART receive busy");
            }
            break;
        case UART_IDLE_BLOCKING:
            ret = HAL_UARTEx_ReceiveToIdle(huart,pData,Size,&idle_rx_len,timeout);
            if(ret == HAL_OK){
                LOG_INFO("UART receive success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART receive error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART receive busy");
            }else
            {
                LOG_WARNING("UART receive timeout");
            }
            break;
        case UART_IDLE_IT:
            ret = HAL_UARTEx_ReceiveToIdle_IT(huart,pData,Size);
            if(ret == HAL_OK){
                LOG_INFO("UART receive success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART receive error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART receive busy");
            }
            break;
        case UART_IDLE_DMA:
            ret = HAL_UARTEx_ReceiveToIdle_DMA(huart,pData,Size);
            if(ret == HAL_OK){
                LOG_INFO("UART receive success");
            }else if (ret==HAL_ERROR)
            {
                LOG_ERROR("UART receive error");
            }else if (ret==HAL_BUSY)
            {
                LOG_ERROR("UART receive busy");
            }
            break;
        default:
            break;
    }
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    for(uint8_t i=0;i<UART_Base::UART_INSTANCES_Index;i++){
        if(UART_Base::UART_INSTANCES[i]->huart==huart){
            UART_Base::UART_INSTANCES[i]->UART_Rx_Callback();
            break;
        }
    }
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    for(uint8_t i=0;i<UART_Base::UART_INSTANCES_Index;i++){
        if(UART_Base::UART_INSTANCES[i]->huart==huart){
            UART_Base::UART_INSTANCES[i]->UART_Rx_Callback();
            break;
        }
    }
}