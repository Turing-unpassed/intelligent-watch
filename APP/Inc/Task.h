#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

/*在此处引用外部文件：       begin*/	
#include "OLED.h"
#include "Time_Display.h"
/*引用外部文件end*/	
void Task_Init(void);
void Task_Loop(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include "W25Q128.h"



#endif

#endif