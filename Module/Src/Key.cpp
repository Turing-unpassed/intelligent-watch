#include "Key.h"

KEY::KEY(GPIO_TypeDef* port,
	uint16_t pin,
	GPIO_PinState activeState,
	uint32_t debounceMs,
	uint32_t longPressMs)
{
	Init(port, pin, activeState, debounceMs, longPressMs);
}

void KEY::Init(GPIO_TypeDef* port,
	uint16_t pin,
	GPIO_PinState activeState,
	uint32_t debounceMs,
	uint32_t longPressMs)
{
	port_ = port;
	pin_ = pin;
	activeState_ = activeState;
	debounceMs_ = debounceMs;
	longPressMs_ = longPressMs;
	Reset();
}

void KEY::Reset()
{
	shortPending_ = false;
	longPending_ = false;
	longFired_ = false;

	uint32_t now = HAL_GetTick();
	rawPressed_ = ReadRawPressed();
	stablePressed_ = rawPressed_;
	lastRawChangeMs_ = now;
	pressStartMs_ = stablePressed_ ? now : 0;
}

void KEY::SetDebounceMs(uint32_t debounceMs)
{
	debounceMs_ = debounceMs;
}

void KEY::SetLongPressMs(uint32_t longPressMs)
{
	longPressMs_ = longPressMs;
}

uint32_t KEY::GetDebounceMs() const
{
	return debounceMs_;
}

uint32_t KEY::GetLongPressMs() const
{
	return longPressMs_;
}

bool KEY::IsPressed() const
{
	return stablePressed_;
}

bool KEY::ConsumeShortPress()
{
	bool ret = shortPending_;
	shortPending_ = false;
	return ret;
}

bool KEY::ConsumeLongPress()
{
	bool ret = longPending_;
	longPending_ = false;
	return ret;
}

void KEY::Update()
{
	Update(HAL_GetTick());
}

void KEY::Update(uint32_t nowMs)
{
	if (port_ == nullptr) {
		return;
	}

	bool newRawPressed = ReadRawPressed();
	if (newRawPressed != rawPressed_) {
		rawPressed_ = newRawPressed;
		lastRawChangeMs_ = nowMs;
	}

	// raw 状态保持足够久后，认为稳定（去抖）
	if (rawPressed_ != stablePressed_) {
		if ((nowMs - lastRawChangeMs_) >= debounceMs_) {
			stablePressed_ = rawPressed_;
			HandleStableEdge(stablePressed_, nowMs);
		}
	}

	// 长按：按住达到阈值即触发一次
	if (stablePressed_ && !longFired_) {
		if ((nowMs - pressStartMs_) >= longPressMs_) {
			longFired_ = true;
			longPending_ = true;
		}
	}
}

bool KEY::ReadRawPressed() const
{
	if (port_ == nullptr) {
		return false;
	}
	return (HAL_GPIO_ReadPin(port_, pin_) == activeState_);
}

void KEY::HandleStableEdge(bool pressed, uint32_t nowMs)
{
	if (pressed) {
		pressStartMs_ = nowMs;
		longFired_ = false;
		return;
	}

	// released
	if (!longFired_) {
		// 未触发长按时，松手即判为短按
		shortPending_ = true;
	}
}

