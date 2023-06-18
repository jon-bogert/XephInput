#include "KeyHandler.h"
#include "InputSystem.h"

using namespace xe;

namespace
{
	bool btwn(const uint8_t val, const uint8_t min, const uint8_t max)
	{
		return (val >= min && val <= max);
	}
}

void xe::KeyHandler::Update()
{
	PollKeys();

	_keyDown = ~_keyHold & _keyBuffer;
	_keyUp = _keyHold & ~_keyBuffer;

	_keyHold = _keyBuffer;
}

void xe::KeyHandler::PollKeys()
{
	_keyBuffer.reset();
	for (uint8_t i = 8; i < 166; ++i)
	{
		//Don't check keys that arent needed
		if (btwn(i, 10, 12) ||
			btwn(i, 14, 15) ||
			btwn(i, 21, 26) ||
			btwn(i, 28, 31) ||
			btwn(i, 41, 43) ||
			i == 47 ||
			btwn(i, 58, 64) ||
			btwn(i, 93, 95) ||
			i == 108 ||
			btwn(i, 124, 159))
			continue;

		SHORT keyState = GetAsyncKeyState(i);
		if (keyState & 0x8000)
			_keyBuffer.set(i);
	}
}

bool xe::KeyHandler::GetKeyHold(Key keycode)
{
	return _keyHold.test(static_cast<uint8_t>(keycode));
}

bool xe::KeyHandler::GetKeyDown(Key keycode)
{
	return _keyDown.test(static_cast<uint8_t>(keycode));
}

bool xe::KeyHandler::GetKeyUp(Key keycode)
{
	return _keyUp.test(static_cast<uint8_t>(keycode));
}
