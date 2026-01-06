#ifndef W25Q128_H
#define W25Q128_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_spi.h"
#include "gpio.h"

#ifdef __cplusplus
}
#endif

#define W25Q128_GB2312_BASE_ADDR					0x00

#ifdef __cplusplus

#define W25Q64_WRITE_ENABLE							0x06
#define W25Q64_WRITE_DISABLE						0x04
#define W25Q64_READ_STATUS_REGISTER_1				0x05
#define W25Q64_READ_STATUS_REGISTER_2				0x35
#define W25Q64_WRITE_STATUS_REGISTER				0x01
#define W25Q64_PAGE_PROGRAM							0x02
#define W25Q64_QUAD_PAGE_PROGRAM					0x32
#define W25Q64_BLOCK_ERASE_64KB						0xD8
#define W25Q64_BLOCK_ERASE_32KB						0x52
#define W25Q64_SECTOR_ERASE_4KB						0x20
#define W25Q64_CHIP_ERASE							0xC7
#define W25Q64_ERASE_SUSPEND						0x75
#define W25Q64_ERASE_RESUME							0x7A
#define W25Q64_POWER_DOWN							0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE				0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID				0x90
#define W25Q64_READ_UNIQUE_ID						0x4B
#define W25Q64_JEDEC_ID								0x9F
#define W25Q64_READ_DATA							0x03
#define W25Q64_FAST_READ							0x0B
#define W25Q64_FAST_READ_DUAL_OUTPUT				0x3B
#define W25Q64_FAST_READ_DUAL_IO					0xBB
#define W25Q64_FAST_READ_QUAD_OUTPUT				0x6B
#define W25Q64_FAST_READ_QUAD_IO					0xEB
#define W25Q64_OCTAL_WORD_READ_QUAD_IO				0xE3
#define W25Q64_DUMMY_BYTE							0xFF

#define W25Q128_PAGE_SIZE							256
#define W25Q128_BUFFER_SIZE						    128
#define W25Q128_Tx_SIZE						        4



//0-216575存放了按照GB2312编码排序的6763个汉字16×16点阵数据（包含5个占位符），每个汉字32字节
//216576-217087为空，方便后续擦除不会破坏汉字点阵数据
//217088-217878存放了表盘图像点阵数据
class W25Q128 : public SPI_Base
{
private:
    uint8_t TxBuffer[W25Q128_Tx_SIZE];

public:
    uint8_t RxBuffer[W25Q128_BUFFER_SIZE];

    W25Q128(SPI_HandleTypeDef* hspi, SPI_Mode mode, uint16_t timeout);
    void WriteEnable();
    void WaitForWriteEnd();
    void ReadData(uint32_t ReadAddr, uint16_t Size);
    void PageProgram(uint32_t WriteAddr,uint8_t* pData,uint16_t Size);
    void SectorErase(uint32_t SectorAddr);
    void ReadJEDECID(uint8_t* MID, uint16_t* DID);
    void ChipErase();
};

#endif

#endif
