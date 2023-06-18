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

int main()
{
	xe::Core::Window window;
	window.Initialize(GetModuleHandle(nullptr), L"Window", 500, 500);
	Vector2 pos;
	HWND hwnd = window.GetWindowHandle();
	xe::InputSystem::Initialize(hwnd);
	//xe::InputSystem::SetCaptureMouse(true);
	while (true)
	{
		xe::InputSystem::Update();

		std::cout << (xe::InputSystem::MouseOverWindow() ? "true" : "false") << std::endl;
		if (xe::InputSystem::GetKeyDown(xe::Key::Esc))
			break;
	}
	window.Terminate();

	return 0;
}