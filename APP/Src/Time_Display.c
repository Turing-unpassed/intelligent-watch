#include "Time_Display.h"

#include "stm32f1xx_hal_rtc_ex.h"

void Time_Display(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    /* STM32F1: HAL keeps date in RAM; persist Y/M/D in BKP regs for reset restore */
    {
        static uint8_t lastYear = 0xFF;
        static uint8_t lastMonth = 0xFF;
        static uint8_t lastDate = 0xFF;

        if (sDate.Year != lastYear || sDate.Month != lastMonth || sDate.Date != lastDate)
        {
            HAL_PWR_EnableBkUpAcces();
            __HAL_RCC_BKP_CLK_ENABLE();

            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, (uint32_t)sDate.Year);
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, (uint32_t)sDate.Month);
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, (uint32_t)sDate.Date);
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0xA5A5u);

            lastYear = sDate.Year;
            lastMonth = sDate.Month;
            lastDate = sDate.Date;
        }
    }

    static const Image_t *tDigits[10] = {&t0, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9};
    static const Image_t *iDigits[10] = {&i0, &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9};

    const int16_t x0 = 13;
    const int16_t y0 = 18;

    const uint8_t tW = 13;
    const uint8_t tH = 30;
    const uint8_t iW = 9;
    const uint8_t iH = 21;

    const int16_t gap = 4;
    const uint8_t colonW = 3;

    uint8_t h = sTime.Hours;
    uint8_t m = sTime.Minutes;
    uint8_t s = sTime.Seconds;

    if (h > 23) h %= 24;
    if (m > 59) m %= 60;
    if (s > 59) s %= 60;

    uint8_t h10 = h / 10;
    uint8_t h01 = h % 10;
    uint8_t m10 = m / 10;
    uint8_t m01 = m % 10;
    uint8_t s10 = s / 10;
    uint8_t s01 = s % 10;

    int16_t x = x0;
    OLED_ShowW25Q128Image(x, y0, tDigits[h10]);
    x += tW + gap;
    OLED_ShowW25Q128Image(x, y0, tDigits[h01]);
    x += tW + gap;

    /* ':' between HH and MM */
    OLED_ClearArea(x, y0, colonW, tH);
    OLED_DrawRectangle(x + 1, y0 + 10, 2, 2, OLED_FILLED);
    OLED_DrawRectangle(x + 1, y0 + 18, 2, 2, OLED_FILLED);
    x += colonW + gap;

    OLED_ShowW25Q128Image(x, y0, tDigits[m10]);
    x += tW + gap;
    OLED_ShowW25Q128Image(x, y0, tDigits[m01]);
    x += tW + gap + 2;

    /* Seconds (SS) using i font, bottom-aligned with HH:MM */
    int16_t ySec = y0 + (int16_t)tH - (int16_t)iH;
    OLED_ShowW25Q128Image(x, ySec, iDigits[s10]);
    x += iW + gap;
    OLED_ShowW25Q128Image(x, ySec, iDigits[s01]);

    OLED_Printf(0,0,OLED_6X8,"%02d-%02d-20%02d",sDate.Date,sDate.Month,sDate.Year);
    OLED_Printf(0,56,OLED_6X8,"Week:%d",sDate.WeekDay);
    OLED_ShowImage(100,0,24,16,Batery);
    OLED_Update();
}