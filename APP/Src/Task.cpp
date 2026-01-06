#include "Task.h"

W25Q128 w25q128(&hspi2, SPI_BLOCKING, 500);
uint8_t MID;
uint16_t DID;

extern "C" void Task_Init(void){
    //初始化代码放在这里
    OLED_Init();

    // w25q128.ReadJEDECID(&MID, &DID);
    // OLED_ShowHexNum(0, 0, MID, 2,OLED_8X16);
    // OLED_ShowHexNum(18, 0, DID, 4,OLED_8X16);
    // OLED_Update();
    
}

extern "C" void Task_Loop(void){
    //循环代码放在这里
    Time_Display();

}