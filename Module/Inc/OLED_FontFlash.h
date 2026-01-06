#ifndef OLED_FONTFLASH_H
#define OLED_FONTFLASH_H

#include "W25Q128.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * W25Q128 字库读取接口（由 C++ 包装实现，供 C 代码 OLED.c 调用）
 */
void OLED_FontFlash_Read(uint32_t addr, uint8_t *dst, uint16_t len);
#define OLED_HZK16_FLASH_BASE		0x000000  // W25Q128 中 GB2312 字库起始地址
#ifdef __cplusplus
}
#endif

#endif
