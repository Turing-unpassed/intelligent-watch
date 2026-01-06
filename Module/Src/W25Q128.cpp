#include "W25Q128.h"

static bool WaitSpiReady(SPI_HandleTypeDef* hspi, uint32_t timeoutMs)
{
    uint32_t start = HAL_GetTick();
    while (HAL_SPI_GetState(hspi) != HAL_SPI_STATE_READY)
    {
        if ((timeoutMs != HAL_MAX_DELAY) && ((HAL_GetTick() - start) > timeoutMs))
        {
            return false;
        }
    }
    return true;
}

W25Q128::W25Q128(SPI_HandleTypeDef* hspi, SPI_Mode mode, uint16_t timeout) : SPI_Base(hspi, mode, timeout){

}

void W25Q128::ReadJEDECID(uint8_t* MID, uint16_t* DID){
    TxBuffer[0] = W25Q64_JEDEC_ID;
    CS_LOW();
    SPI_Send(TxBuffer, 1);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 ReadJEDECID SPI busy timeout");
        CS_HIGH();
        return;
    }
    SPI_Receive(MID, 1);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 ReadJEDECID SPI busy timeout");
        CS_HIGH();
        return;
    }
    uint8_t did_bytes[2] = {0};
    SPI_Receive(did_bytes, 2);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 ReadJEDECID SPI busy timeout");
        CS_HIGH();
        return;
    }
    *DID = (uint16_t)(did_bytes[0] << 8) | did_bytes[1];
    CS_HIGH();
}

void W25Q128::WriteEnable(){
    TxBuffer[0] = W25Q64_WRITE_ENABLE;
    CS_LOW();
    SPI_Send(TxBuffer, 1);
    (void)WaitSpiReady(hspi, timeout);
    CS_HIGH();
}

void W25Q128::ReadData(uint32_t ReadAddr, uint16_t Size){
    if (Size > sizeof(RxBuffer))
    {
        LOG_WARNING("W25Q128 ReadData truncated to RxBuffer size");
        Size = sizeof(RxBuffer);
    }
    TxBuffer[0] = W25Q64_READ_DATA;
    TxBuffer[1] = (ReadAddr >> 16) & 0xFF;
    TxBuffer[2] = (ReadAddr >> 8) & 0xFF;
    TxBuffer[3] = ReadAddr & 0xFF;
    CS_LOW();
    SPI_Send(TxBuffer, 4);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 ReadData SPI busy timeout");
        CS_HIGH();
        return;
    }
    SPI_Receive(RxBuffer, Size);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 ReadData SPI busy timeout");
        CS_HIGH();
        return;
    }
    CS_HIGH();
}

void W25Q128::WaitForWriteEnd(){
    uint32_t start_time = HAL_GetTick();
    uint8_t status=0;
    TxBuffer[0] = W25Q64_READ_STATUS_REGISTER_1;
    CS_LOW();
    SPI_Send(TxBuffer, 1);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 WaitForWriteEnd SPI busy timeout");
        CS_HIGH();
        return;
    }
    SPI_Receive(&status, 1);
    if (!WaitSpiReady(hspi, timeout))
    {
        LOG_ERROR("W25Q128 WaitForWriteEnd SPI busy timeout");
        CS_HIGH();
        return;
    }
    while((status & 0x01) == 0x01){
        if((HAL_GetTick() - start_time) > timeout){
            LOG_ERROR("W25Q128 WaitForWriteEnd Timeout");
            break;
        }
        SPI_Receive(&status, 1);
        if (!WaitSpiReady(hspi, timeout))
        {
            LOG_ERROR("W25Q128 WaitForWriteEnd SPI busy timeout");
            break;
        }
    }
    CS_HIGH();
}

void W25Q128::SectorErase(uint32_t SectorAddr){
    WriteEnable();
    TxBuffer[0] = W25Q64_SECTOR_ERASE_4KB;
    TxBuffer[1] = (SectorAddr >> 16) & 0xFF;
    TxBuffer[2] = (SectorAddr >> 8) & 0xFF;
    TxBuffer[3] = SectorAddr & 0xFF;
    CS_LOW();
    SPI_Send(TxBuffer, 4);
    (void)WaitSpiReady(hspi, timeout);
    CS_HIGH();
    WaitForWriteEnd();
}

void W25Q128::PageProgram(uint32_t WriteAddr,uint8_t* pData,uint16_t Size){
    if ((pData == nullptr) || (Size == 0U))
    {
        return;
    }

    uint32_t currentAddr = WriteAddr;
    uint32_t remaining = Size;
    uint8_t* currentData = pData;

    while (remaining > 0U)
    {
        uint32_t pageOffset = currentAddr % W25Q128_PAGE_SIZE;
        uint16_t chunk = (uint16_t)(W25Q128_PAGE_SIZE - pageOffset);
        if (chunk > remaining)
        {
            chunk = (uint16_t)remaining;
        }

        WriteEnable();
        TxBuffer[0] = W25Q64_PAGE_PROGRAM;
        TxBuffer[1] = (currentAddr >> 16) & 0xFF;
        TxBuffer[2] = (currentAddr >> 8) & 0xFF;
        TxBuffer[3] = currentAddr & 0xFF;

        CS_LOW();
        SPI_Send(TxBuffer, 4);
        if (!WaitSpiReady(hspi, timeout))
        {
            LOG_ERROR("W25Q128 PageProgram SPI busy timeout");
            CS_HIGH();
            return;
        }

        SPI_Send(currentData, chunk);
        if (!WaitSpiReady(hspi, timeout))
        {
            LOG_ERROR("W25Q128 PageProgram SPI busy timeout");
            CS_HIGH();
            return;
        }
        CS_HIGH();

        WaitForWriteEnd();

        currentAddr += chunk;
        currentData += chunk;
        remaining -= chunk;
        LOG_INFO("%d bytes left", remaining);
    }
}

void W25Q128::ChipErase(){
    WriteEnable();
    TxBuffer[0] = W25Q64_CHIP_ERASE;
    LOG_INFO("W25Q128 Chip Erase Start");
    CS_LOW();
    SPI_Send(TxBuffer, 1);
    (void)WaitSpiReady(hspi, timeout);
    CS_HIGH();
    WaitForWriteEnd();
    LOG_INFO("W25Q128 Chip Erase Completed");
}