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
public:
	void Print(xe::InputAction* ctx)
	{
		std::cout << "BAM" << std::endl;
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
	xe::InputAction* action = actionMap.CreateAction("Jump", xe::InputAction::Type::Button);
	action->performed.Subscribe(&tc, std::bind(&TestClass::Print, &tc, std::placeholders::_1));
	action->AddButton(xe::Gamepad::Button::A);
	action->AddButton(xe::Key::Space);

	while (true)
	{
		xe::InputSystem::Update();
		actionMap.Update();

		xe::InputSystem::GetKeyAxisComposite2D(&pos.x, xe::Key::A, xe::Key::D, xe::Key::S, xe::Key::W);

		std::cout << pos.x << " " << pos.y << std::endl;
	}
	window.Terminate();

	return 0;
}