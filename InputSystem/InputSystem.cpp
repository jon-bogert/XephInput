#include "InputSystem.h"
#include "KeyHandler.h"

using namespace xe;

#define BUTTONAXIS_LT 0x1000
#define BUTTONAXIS_RT 0x2000
#define BUTTONAXIS_LS_UP 0x4000
#define BUTTONAXIS_LS_DOWN 0x8000
#define BUTTONAXIS_LS_LEFT 0x0100
#define BUTTONAXIS_LS_RIGHT 0x0200
#define BUTTONAXIS_RS_UP 0x0400
#define BUTTONAXIS_RS_DOWN 0x0800
#define BUTTONAXIS_RS_LEFT 0x0010
#define BUTTONAXIS_RS_RIGHT 0x0020

namespace
{
	const float BYTE_2_FLT = 1.f / 255.f;
	const float STICK_2_FLT = 1.f / 32767.5;
	const float TRI_MAG = 1.f / std::sqrtf(2.f);

	float ApplyDeadzone(float val, float inMin, float inMax)
	{
		bool isNeg = (val < 0.f);
		if (isNeg) val *= -1;
		if (val < inMin) val = inMin;
		float rangeFactor = 1.f / (inMax - inMin);
		float result = (val - inMin) * rangeFactor;
		if (isNeg) result *= -1.f;
		return  result;
	}

	bool GamepadXInput(Gamepad::Button button, WORD& out) //returns true if normal button
	{
		switch (button)
		{
		case Gamepad::Button::A:
			out = XINPUT_GAMEPAD_A;
			return true;
		case Gamepad::Button::B:
			out = XINPUT_GAMEPAD_B;
			return true;
		case Gamepad::Button::X:
			out = XINPUT_GAMEPAD_X;
			return true;
		case Gamepad::Button::Y:
			out = XINPUT_GAMEPAD_Y;
			return true;
		case Gamepad::Button::LB:
			out = XINPUT_GAMEPAD_LEFT_SHOULDER;
			return true;
		case Gamepad::Button::RB:
			out = XINPUT_GAMEPAD_RIGHT_SHOULDER;
			return true;
		case Gamepad::Button::LS_Press:
			out = XINPUT_GAMEPAD_LEFT_THUMB;
			return true;
		case Gamepad::Button::RS_Press:
			out = XINPUT_GAMEPAD_RIGHT_THUMB;
			return true;
		case Gamepad::Button::Start:
			out = XINPUT_GAMEPAD_START;
			return true;
		case Gamepad::Button::Select:
			out = XINPUT_GAMEPAD_BACK;
			return true;
		case Gamepad::Button::DPad_Up:
			out = XINPUT_GAMEPAD_DPAD_UP;
			return true;
		case Gamepad::Button::DPad_Down:
			out = XINPUT_GAMEPAD_DPAD_DOWN;
			return true;
		case Gamepad::Button::DPad_Left:
			out = XINPUT_GAMEPAD_DPAD_LEFT;
			return true;
		case Gamepad::Button::DPad_Right:
			out = XINPUT_GAMEPAD_DPAD_RIGHT;
			return true;
			//===================================================
		case Gamepad::Button::LT:
			out = BUTTONAXIS_LT;
			return false;
		case Gamepad::Button::RT:
			out = BUTTONAXIS_RT;
			return false;
		case Gamepad::Button::LS_Up:
			out = BUTTONAXIS_LS_UP;
			return false;
		case Gamepad::Button::LS_Down:
			out = BUTTONAXIS_LS_DOWN;
			return false;
		case Gamepad::Button::LS_Left:
			out = BUTTONAXIS_LS_LEFT;
			return false;
		case Gamepad::Button::LS_Right:
			out = BUTTONAXIS_LS_RIGHT;
			return false;
		case Gamepad::Button::RS_Up:
			out = BUTTONAXIS_RS_UP;
			return false;
		case Gamepad::Button::RS_Down:
			out = BUTTONAXIS_RS_DOWN;
			return false;
		case Gamepad::Button::RS_Left:
			out = BUTTONAXIS_RS_LEFT;
			return false;
		case Gamepad::Button::RS_Right:
			out = BUTTONAXIS_RS_RIGHT;
			return false;
		default:
			std::cout << "Bad Input" << std::endl;
			return 0;
		}
	}

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
	return Get()._IsControllerConnected(num);
}

bool xe::InputSystem::GetGamepadHold(Gamepad::Button button, uint8_t num)
{
	return Get()._GetGamepadHold(button, num);
}

bool xe::InputSystem::GetGamepadDown(Gamepad::Button button, uint8_t num)
{
	return Get()._GetGamepadDown(button, num);
}

bool xe::InputSystem::GetGamepadUp(Gamepad::Button button, uint8_t num)
{
	return Get()._GetGamepadUp(button, num);
}

void xe::InputSystem::GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num)
{
	Get()._GetGamepadAxis(out_v2, axis, num);
}

float xe::InputSystem::GetTriggerThreshold()
{
	return Get()._GetTriggerThreshold();
}

void xe::InputSystem::SetTriggerThreshold(const float threshold)
{
	Get()._SetTriggerThreshold(threshold);
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

//================================================================================
//                       IMPLEMENTATION
//================================================================================


void xe::InputSystem::_Initialize(HWND& hwnd)
{
	_hwnd = hwnd;

	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		DWORD result = XInputGetState(i, &_state);
		if (result == ERROR_SUCCESS)
			_controllerCount++;
	}
	for (uint8_t i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		_controllerButtonHold[i] = 0;
		_controllerButtonDown[i] = 0;
		_controllerButtonUp[i] = 0;
	}
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

	//if (!_controllerActive) return; 
	uint8_t controllerCount = 0;
	DWORD result;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		ZeroMemory(&_state, sizeof(XINPUT_STATE));
		result = XInputGetState(i, &_state);
		if (result == ERROR_SUCCESS)
		{
			controllerCount++;
			WORD buttonState = _state.Gamepad.wButtons;

			//Get button/up down
			_controllerButtonDown[i] = ~_controllerButtonHold[i] & buttonState;
			_controllerButtonUp[i] = _controllerButtonHold[i] & ~buttonState;

			//Store current values
			_controllerButtonHold[i] = buttonState;

			// Update axis
			_controllerAxisState[i].lx = (static_cast<short>(_state.Gamepad.sThumbLX) + 0.5f) * STICK_2_FLT;
			_controllerAxisState[i].ly = (static_cast<short>(_state.Gamepad.sThumbLY) + 0.5f) * STICK_2_FLT;
			_controllerAxisState[i].rx = (static_cast<short>(_state.Gamepad.sThumbRX) + 0.5f) * STICK_2_FLT;
			_controllerAxisState[i].ry = (static_cast<short>(_state.Gamepad.sThumbRY) + 0.5f) * STICK_2_FLT;

			_controllerAxisState[i].lt = static_cast<uint8_t>(_state.Gamepad.bLeftTrigger) * BYTE_2_FLT;
			_controllerAxisState[i].rt = static_cast<uint8_t>(_state.Gamepad.bRightTrigger) * BYTE_2_FLT;

			UpdateAxisState(i);
		}
	}
	if (_controllerCount != controllerCount)
	{
		_controllerCountChange = true;
		_controllerCount = controllerCount;
	}
	_keyHandler->Update();
}

bool xe::InputSystem::_IsControllerConnected(const uint8_t num) const
{
	return num < _controllerCount;
}

bool xe::InputSystem::_GetGamepadHold(Gamepad::Button button, uint8_t num)
{
	WORD result;
	if (GamepadXInput(button, result))
	{
		return _controllerButtonHold[num] & result;
	}
	return _controllerAxisHold[num] & result;
}

bool xe::InputSystem::_GetGamepadDown(Gamepad::Button button, uint8_t num)
{
	WORD result;
	if (GamepadXInput(button, result))
	{
		return _controllerButtonDown[num] & result;
	}
	return _controllerAxisDown[num] & result;
}

bool xe::InputSystem::_GetGamepadUp(Gamepad::Button button, uint8_t num)
{
	WORD result;
	if (GamepadXInput(button, result))
	{
		return _controllerButtonUp[num] & result;
	}
	return _controllerAxisUp[num] & result;
}

void xe::InputSystem::_GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num)
{
	switch (axis)
	{
	case Gamepad::Axis::LS:
		*out_v2 = ApplyDeadzone(_controllerAxisState[num].lx, _deadzoneMinimum, _deadzoneMaximum);
		*(out_v2 + 1) = ApplyDeadzone(_controllerAxisState[num].ly, _deadzoneMinimum, _deadzoneMaximum);
		return;
	case Gamepad::Axis::RS:
		*out_v2 = ApplyDeadzone(_controllerAxisState[num].rx, _deadzoneMinimum, _deadzoneMaximum);
		*(out_v2 + 1) = ApplyDeadzone(_controllerAxisState[num].ry, _deadzoneMinimum, _deadzoneMaximum);
		return;
	case Gamepad::Axis::Trig:
		*out_v2 = _controllerAxisState[num].lt;
		*(out_v2 + 1) = _controllerAxisState[num].rt;
		return;
	case Gamepad::Axis::DPad:
		float x = (float)_GetGamepadHold(Gamepad::Button::DPad_Right) - (float)_GetGamepadHold(Gamepad::Button::DPad_Left);
		float y = (float)_GetGamepadHold(Gamepad::Button::DPad_Up) - (float)_GetGamepadHold(Gamepad::Button::DPad_Down);
		if (std::fabsf(x) > FLT_EPSILON && std::fabsf(y) > FLT_EPSILON)
		{
			x *= TRI_MAG;
			y *= TRI_MAG;
		}
		*out_v2 = x;
		*(out_v2 + 1) = y;
		return;
	}
}

float xe::InputSystem::_GetTriggerThreshold() const
{
	return _triggerThreshold;
}

void xe::InputSystem::_SetTriggerThreshold(const float threshold)
{
	_triggerThreshold = threshold;
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

void xe::InputSystem::UpdateAxisState(uint8_t index)
{
	WORD newState = 0;
	if (_controllerAxisState[index].lt >= _triggerThreshold)
		newState |= BUTTONAXIS_LT;
	if (_controllerAxisState[index].rt >= _triggerThreshold)
		newState |= BUTTONAXIS_RT;

	if (_controllerAxisState[index].ly >= _deadzoneMinimum)
		newState |= BUTTONAXIS_LS_UP;
	if (_controllerAxisState[index].ly <= -_deadzoneMinimum)
		newState |= BUTTONAXIS_LS_DOWN;
	if (_controllerAxisState[index].lx >= _deadzoneMinimum)
		newState |= BUTTONAXIS_LS_RIGHT;
	if (_controllerAxisState[index].lx <= -_deadzoneMinimum)
		newState |= BUTTONAXIS_LS_LEFT;

	if (_controllerAxisState[index].ry >= _deadzoneMinimum)
		newState |= BUTTONAXIS_RS_UP;
	if (_controllerAxisState[index].ry <= -_deadzoneMinimum)
		newState |= BUTTONAXIS_RS_DOWN;
	if (_controllerAxisState[index].rx >= _deadzoneMinimum)
		newState |= BUTTONAXIS_RS_RIGHT;
	if (_controllerAxisState[index].rx <= -_deadzoneMinimum)
		newState |= BUTTONAXIS_RS_LEFT;

	//Get button/up down
	_controllerAxisDown[index] = ~_controllerAxisHold[index] & newState;
	_controllerAxisUp[index] = _controllerAxisHold[index] & ~newState;

	//Store current values
	_controllerAxisHold[index] = newState;
}
