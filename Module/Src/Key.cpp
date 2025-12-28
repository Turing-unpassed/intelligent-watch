#include "Key.h"

KEY::KEY(GPIO_TypeDef* port, uint16_t pin, uint16_t long_press_time)
    : port(port), pin(pin), long_press_time(long_press_time),
      press_time(0), is_short_pressed(false), is_long_pressed(false)
{

}

KEY::~KEY(){}
void KEY::isPressed(){
    if(HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET){
        press_time = HAL_GetTick();
        HAL_Delay(10);
    }
}