/* ==================================================================================
* MIT License
*
* Copyright (c) 2023 Jon Bogert
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
================================================================================== */

#include "KeyHandler.h"

#include "XephInput/InputSystem.h"

using namespace xe;

namespace
{
	const float NORMALIZE_AXIS = 1.f/sqrtf(2.f);
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
	for (uint8_t i = 1; i < 166; ++i)
	{
		//Don't check keys that arent needed
		if (i == 3 ||
			i == 7 ||
			btwn(i, 10, 12) ||
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

bool xe::KeyHandler::GetMouseHold(Mouse::Button btncode)
{
	return _keyHold.test(static_cast<uint8_t>(btncode));
}

bool xe::KeyHandler::GetMouseDown(Mouse::Button btncode)
{
	return _keyDown.test(static_cast<uint8_t>(btncode));
}

bool xe::KeyHandler::GetMouseUp(Mouse::Button btncode)
{
	return _keyUp.test(static_cast<uint8_t>(btncode));
}

float xe::KeyHandler::GetKeyAxisComposite1D(Key negative, Key positive)
{
	float neg = static_cast<float>(GetKeyHold(negative));
	float pos = static_cast<float>(GetKeyHold(positive));
	return pos - neg;
}

void xe::KeyHandler::GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY)
{
	out_v2[0] = GetKeyAxisComposite1D(negX, posX);
	out_v2[1] = GetKeyAxisComposite1D(negY, posY);
	if (out_v2[0] != 0.f && out_v2[1] != 0.f)
	{
		out_v2[0] *= NORMALIZE_AXIS;
		out_v2[1] *= NORMALIZE_AXIS;
	}
}
