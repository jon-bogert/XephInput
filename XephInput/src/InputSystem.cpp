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

#include "XephInput/InputSystem.h"

#include "GamepadHandler.h"
#include "KeyHandler.h"

using namespace xe;

namespace
{
	POINT WindowCenter(HWND hwnd)
	{
		POINT pnt;
		RECT windowRect;
		GetWindowRect(hwnd, &windowRect);
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		int halfTitleBarHeight = (windowRect.bottom - windowRect.top - clientRect.bottom - clientRect.top) * 0.5f;
		// Calculate the center position
		pnt.x = (int)(windowRect.left + (windowRect.right - windowRect.left) * 0.5f);
		pnt.y = (int)(windowRect.top + (windowRect.bottom - windowRect.top) * 0.5f + halfTitleBarHeight);
		return pnt;
	}
}

xe::InputSystem::InputSystem()
{
	_gamepadHandler = new GamepadHandler();
	_keyHandler = new KeyHandler();
	GetCursorPos(&_mousePos);
}

InputSystem& xe::InputSystem::Get()
{
	static InputSystem inst;
	return inst;
}

xe::InputSystem::~InputSystem()
{
	delete _keyHandler;
	delete _gamepadHandler;
}

//================================================================================
//                       STATIC INTERFACES
//================================================================================


void xe::InputSystem::Initialize(HWND& hwnd)
{
	Get()._Initialize(hwnd);
}

void xe::InputSystem::Update()
{
	Get()._Update();
}

bool xe::InputSystem::IsControllerConnected(const uint8_t num)
{
	return Get()._gamepadHandler->IsControllerConnected(num);
}

bool xe::InputSystem::GetGamepadHold(Gamepad::Button button, uint8_t num)
{
	return Get()._gamepadHandler->GetGamepadHold(button, num);
}

bool xe::InputSystem::GetGamepadDown(Gamepad::Button button, uint8_t num)
{
	return Get()._gamepadHandler->GetGamepadDown(button, num);
}

bool xe::InputSystem::GetGamepadUp(Gamepad::Button button, uint8_t num)
{
	return Get()._gamepadHandler->GetGamepadUp(button, num);
}

void xe::InputSystem::GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num)
{
	Get()._gamepadHandler->GetGamepadAxis(out_v2, axis, num);
}

float xe::InputSystem::GetTriggerThreshold()
{
	return Get()._gamepadHandler->GetTriggerThreshold();
}

void xe::InputSystem::SetTriggerThreshold(const float threshold)
{
	Get()._gamepadHandler->SetTriggerThreshold(threshold);
}

bool xe::InputSystem::GetKeyHold(Key keycode)
{
	return Get()._keyHandler->GetKeyHold(keycode);
}

bool xe::InputSystem::GetKeyDown(Key keycode)
{
	return Get()._keyHandler->GetKeyDown(keycode);
}

bool xe::InputSystem::GetKeyUp(Key keycode)
{
	return Get()._keyHandler->GetKeyUp(keycode);
}

bool xe::InputSystem::GetMouseHold(Mouse::Button btncode)
{
	return Get()._keyHandler->GetMouseHold(btncode);
}

bool xe::InputSystem::GetMouseDown(Mouse::Button btncode)
{
	return Get()._keyHandler->GetMouseDown(btncode);
}

bool xe::InputSystem::GetMouseUp(Mouse::Button btncode)
{
	return Get()._keyHandler->GetMouseUp(btncode);
}

float xe::InputSystem::GetKeyAxisComposite1D(Key negative, Key positive)
{
	return Get()._keyHandler->GetKeyAxisComposite1D(negative, positive);
}

void xe::InputSystem::GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY)
{
	return Get()._keyHandler->GetKeyAxisComposite2D(out_v2, negX, posX, negY, posY);
}

void xe::InputSystem::GetMousePos(float* out_v2, bool relativeToWindow)
{
	Get()._GetMousePos(out_v2, relativeToWindow);
}

void xe::InputSystem::GetMouseDelta(float* out_v2)
{
	out_v2[0] = Get()._mouseDelta[0];
	out_v2[1] = Get()._mouseDelta[1];
}

void xe::InputSystem::SetCaptureMouse(const bool captureMouse)
{
	if (!Get()._hwnd) return;
	Get()._captureMouse = captureMouse;
}

bool xe::InputSystem::GetCaptureMouse()
{
	return Get()._captureMouse;
}

bool xe::InputSystem::MouseOverWindow()
{
	if (!Get()._hwnd) return false;
	float pos[2];
	RECT clientRect;
	GetMousePos(pos, true);
	GetClientRect(Get()._hwnd, &clientRect);
	return !(pos[0] < 0 || pos[0] >= (float)clientRect.bottom || pos[1] < 0 || pos[1] >= (float)clientRect.right);
}

xe::InputActionMap* xe::InputSystem::CreateInputActionMap(std::string name)
{
	InputActionMap* map = Get()._actionMaps.emplace_back(std::make_unique<InputActionMap>()).get();
	map->name = name;
	return map;
}

InputActionMap* xe::InputSystem::FindInputActionMap(std::string name)
{
	auto it = std::find_if(Get()._actionMaps.begin(), Get()._actionMaps.end(), [=](const std::unique_ptr<InputActionMap>& x) {return x->name == name; });
	return (it == Get()._actionMaps.end()) ? nullptr : it->get();
}

//================================================================================
//                       IMPLEMENTATION
//================================================================================


void xe::InputSystem::_Initialize(HWND& hwnd)
{
	_hwnd = hwnd;
	_gamepadHandler->Initialize();
}

void xe::InputSystem::_Update()
{
	POINT newPos;
	GetCursorPos(&newPos);
	if (_captureMouse)
	{
		if (_hwnd == nullptr) return; // Cannot capture mouse while window is null
		POINT center = WindowCenter(_hwnd);

		_mouseDelta[0] = newPos.x - center.x;
		_mouseDelta[1] = newPos.y - center.y;
		SetCursorPos(center.x, center.y);
	}
	else
	{
		_mouseDelta[0] = newPos.x - _mousePos.x;
		_mouseDelta[1] = newPos.y - _mousePos.y;
		_mousePos = newPos;
	}

	_gamepadHandler->Update();
	_keyHandler->Update();
	for (auto& map : _actionMaps)
		map->Update();
}

void xe::InputSystem::_GetMousePos(float* out_v2, bool relativeToWindow)
{
	if (_hwnd && relativeToWindow)
	{
		POINT copy = _mousePos;
		ScreenToClient(_hwnd, &copy);
		out_v2[0] = copy.x;
		out_v2[1] = copy.y;
	}
	else
	{
		out_v2[0] = _mousePos.x;
		out_v2[1] = _mousePos.y;
	}

}
