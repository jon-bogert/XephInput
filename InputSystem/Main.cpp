#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "XInput.lib")

#include "InputSystem.h"
#include "InputActionMap.h"

struct Vector2
{
	float x = 0;
	float y = 0;
};

class TestClass
{
	Vector2 position;
	float val;
public:
	void OnJump(xe::InputAction* ctx)
	{
		std::cout << "BAM" << std::endl;
	}
};

class TestClass2
{
public:
	void OnDash(xe::InputAction* ctx)
	{
		std::cout << "BAM2" << std::endl;
	}
};

int main()
{
	HWND hwnd;
	xe::InputSystem::Initialize(hwnd);
	Vector2 axis;
	xe::InputActionMap map;
	xe::InputAction* jump = map.CreateAction("Jump", xe::InputAction::Type::Button);
	jump->AddButton(xe::Gamepad::Button::A);

	TestClass tc;
	TestClass2 tc2;
	jump->performed.Subscribe(XEInputActionCallbackPtr(TestClass::OnJump, &tc));
	jump->performed.Subscribe(XEInputActionCallbackPtr(TestClass2::OnDash, &tc2));

	while (true)
	{
		xe::InputSystem::Update();
		map.Update();
		if (xe::InputSystem::GetGamepadDown(xe::Gamepad::Button::Start))
		{
			jump->performed.UnsubscribeAll(&tc);
		}
	}
	return 0;
}