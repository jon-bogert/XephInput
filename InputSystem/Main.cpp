#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#pragma comment(lib, "XInput.lib")

#include "InputSystem.h"
#include "InputActionMap.h"
#include "Window.h"

std::bitset<256> store;
struct Vector2
{
	float x;
	float y;
};

class TestClass
{
	Vector2 pos = { 0, 0 };
public:
	void Print(xe::InputAction* ctx)
	{
		ctx->ReadValue(&pos.x);
		std::cout << pos.x << " " << pos.y << std::endl;
	}
};

int main()
{
	xe::Core::Window window;
	window.Initialize(GetModuleHandle(nullptr), L"Window", 500, 500);
	Vector2 pos;
	HWND hwnd = window.GetWindowHandle();
	xe::InputSystem::Initialize(hwnd);

	TestClass tc;
	xe::InputActionMap actionMap;
	xe::InputAction* action = actionMap.CreateAction("Move", xe::InputAction::Type::Axis2D);
	action->performed.Subscribe(&tc, std::bind(&TestClass::Print, &tc, std::placeholders::_1));
	action->Add2DAxis(xe::Gamepad::Axis::LS);
	action->Add2DAxis(xe::Key::A, xe::Key::D, xe::Key::S, xe::Key::W);

	while (true)
	{
		xe::InputSystem::Update();
		actionMap.Update();
	}
	window.Terminate();

	return 0;
}