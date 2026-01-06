#include "OLED_FontFlash.h"

#include <cstring>

#include "W25Q128.h"

// Defined in APP/Src/Task.cpp
extern W25Q128 w25q128;

extern "C" void OLED_Flash_Read(uint32_t addr, uint8_t *dst, uint16_t len)
{
    if ((dst == nullptr) || (len == 0U))
    {
        return;
    }

    w25q128.ReadData(addr, len);
    std::memcpy(dst, w25q128.RxBuffer, len);
}
