#ifndef KEY_H
#define KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

/*在此处引用外部文件：       begin*/	
#include <stdint.h>
#include "gpio.h"

/*引用外部文件end*/	

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class KEY
{
public:
    KEY(GPIO_TypeDef* port, uint16_t pin,uint16_t long_press_time=600);
    ~KEY();
    void isPressed();
private:
    GPIO_TypeDef* port;
    uint16_t pin;
    uint16_t long_press_time;
    uint32_t press_time;
    bool is_short_pressed;
    bool is_long_pressed;
    
};

#endif

#endif