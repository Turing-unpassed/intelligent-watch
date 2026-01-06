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
	// 默认参数：当前硬件配置为上拉输入 + 下降沿中断，因此按下为低电平
	static constexpr uint32_t DEFAULT_DEBOUNCE_MS = 20;
	static constexpr uint32_t DEFAULT_LONGPRESS_MS = 800;

	KEY() = default;
	KEY(GPIO_TypeDef* port,
		uint16_t pin,
		GPIO_PinState activeState = GPIO_PIN_RESET,
		uint32_t debounceMs = DEFAULT_DEBOUNCE_MS,
		uint32_t longPressMs = DEFAULT_LONGPRESS_MS);

	void Init(GPIO_TypeDef* port,
		uint16_t pin,
		GPIO_PinState activeState = GPIO_PIN_RESET,
		uint32_t debounceMs = DEFAULT_DEBOUNCE_MS,
		uint32_t longPressMs = DEFAULT_LONGPRESS_MS);

	// 周期调用（建议 1~10ms 级别，或主循环每次都调）
	void Update();
	void Update(uint32_t nowMs);

	// 一次性事件读取：读取到 true 后会自动清除
	bool ConsumeShortPress();
	bool ConsumeLongPress();

	bool IsPressed() const;
	void Reset();

	void SetDebounceMs(uint32_t debounceMs);
	void SetLongPressMs(uint32_t longPressMs);
	uint32_t GetDebounceMs() const;
	uint32_t GetLongPressMs() const;

private:
	bool ReadRawPressed() const;
	void HandleStableEdge(bool pressed, uint32_t nowMs);

	GPIO_TypeDef* port_ = nullptr;
	uint16_t pin_ = 0;
	GPIO_PinState activeState_ = GPIO_PIN_RESET;

	uint32_t debounceMs_ = DEFAULT_DEBOUNCE_MS;
	uint32_t longPressMs_ = DEFAULT_LONGPRESS_MS;

	bool rawPressed_ = false;
	bool stablePressed_ = false;
	uint32_t lastRawChangeMs_ = 0;
	uint32_t pressStartMs_ = 0;
	bool longFired_ = false;

	bool shortPending_ = false;
	bool longPending_ = false;

    
};

#endif

#endif