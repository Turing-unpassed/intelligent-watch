#include "bsp_spi.h"

SPI_Base* SPI_Base::SPI_BUS[MAX_SPI_BUS_NUM] = {nullptr};
uint8_t SPI_Base::SPI_BUS_Index = 0;

SPI_Base::SPI_Base(SPI_HandleTypeDef* hspi, SPI_Mode mode, uint16_t timeout)
{
	this->hspi = hspi;
	this->mode = mode;
	this->timeout = timeout;

	if (SPI_BUS_Index < MAX_SPI_BUS_NUM)
	{
		SPI_BUS[SPI_BUS_Index++] = this;
	}
	else
	{
		LOG_WARNING("SPI Instances exceed MAX_SPI_BUS_NUM");
	}
}

void SPI_Base::SPI_Send(uint8_t* pData, uint16_t Size)
{
	HAL_StatusTypeDef ret;
	switch (mode)
	{
		case SPI_BLOCKING:
			ret = HAL_SPI_Transmit(hspi, pData, Size, timeout);
			if (ret != HAL_OK)
			{
				LOG_ERROR("SPI Transmit Error");
			}
			break;
		case SPI_IT:
			ret = HAL_SPI_Transmit_IT(hspi, pData, Size);
			if (ret != HAL_OK)
			{
				LOG_ERROR("SPI Transmit IT Error");
			}
			break;
		case SPI_DMA:
			ret = HAL_SPI_Transmit_DMA(hspi, pData, Size);
			if (ret != HAL_OK)
			{
				LOG_ERROR("SPI Transmit DMA Error");
			}
			break;
		default:
			break;
	}
}

void SPI_Base::SPI_Receive(uint8_t* pData, uint16_t Size)
{
	HAL_StatusTypeDef ret;
	switch (mode)
	{
		case SPI_BLOCKING:
			ret = HAL_SPI_Receive(hspi, pData, Size, timeout);
			if (ret != HAL_OK)
			{
				LOG_ERROR("SPI Receive Error");
			}
			break;
		case SPI_IT:
			ret = HAL_SPI_Receive_IT(hspi, pData, Size);
			if (ret != HAL_OK)
			{
				LOG_ERROR("SPI Receive IT Error");
			}
			break;
		case SPI_DMA:
			ret = HAL_SPI_Receive_DMA(hspi, pData, Size);
			if (ret != HAL_OK)
			{
				LOG_ERROR("SPI Receive DMA Error");
			}
			break;
		default:
			break;
	}
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	for (uint8_t i = 0; i < SPI_Base::SPI_BUS_Index; i++)
	{
		if (SPI_Base::SPI_BUS[i]->hspi == hspi)
		{
			SPI_Base::SPI_BUS[i]->SPI_Rx_Callback();
			break;
		}
	}
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	for (uint8_t i = 0; i < SPI_Base::SPI_BUS_Index; i++)
	{
		if (SPI_Base::SPI_BUS[i]->hspi == hspi)
		{
			SPI_Base::SPI_BUS[i]->SPI_Rx_Callback();
			break;
		}
	}
}
