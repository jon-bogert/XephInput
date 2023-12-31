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

#include <iostream>

#include "XephInput/InputSystem.h"
#include "Window.h"

/*
* Demo Scene showing basic axis input
* 
* Showing Devices:
*	- Gamepad
*	- Multiplayer Gamepad
*   - Keyboard Composites
*
* Shows 1D and 2D Axis
*/

//Custom standard struct class (or from any library)
struct Vector2
{
	float x;
	float y;
};

int main(int argc, char* argv[])
{
	//Initialize Demo Window
	Window window;
	window.Initialize(GetModuleHandle(nullptr), L"Window", 500, 500);
	HWND hwnd = window.GetWindowHandle();

	//Initialize InputSystem with demo window;
	xe::InputSystem::Initialize(hwnd);

	std::cout << "Press ESC at any time to quit..." << std::endl;
	std::cout << "Press 1-3 for test cases:" << std::endl;
	std::cout << "  [1] Check if mouse is over window space" << std::endl;
	std::cout << "  [2] Get Mouse Position Relative to Window" << std::endl;
	std::cout << "  [3] Get Mouse Position in Screen Space" << std::endl;
	std::cout << "  [4] Mouse Delta & Capture Mouse" << std::endl;

	int testCase = 0;
	Vector2 mousePos;

	//Application Loop
	while (true)
	{
		xe::InputSystem::Update(); //Update all inputs this frame

		if (xe::InputSystem::GetKeyDown(xe::Key::Esc)) // Close window if ESC is pressed
			break;

		     if (xe::InputSystem::GetKeyDown(xe::Key::One))	 testCase = 1;
		else if (xe::InputSystem::GetKeyDown(xe::Key::Two))	 testCase = 2;
		else if (xe::InputSystem::GetKeyDown(xe::Key::Three)) testCase = 3;
		else if (xe::InputSystem::GetKeyDown(xe::Key::Four))	 testCase = 4;

		switch (testCase)
		{
			// 1 - Check if mouse is over window space
		case 1:
			if (xe::InputSystem::GetCaptureMouse()) xe::InputSystem::SetCaptureMouse(false);
			if (xe::InputSystem::MouseOverWindow())
				std::cout << "Mouse over window" << std::endl;
			break;

			// 2 - Get Mouse Position Relative to Window
		case 2:
			if (xe::InputSystem::GetCaptureMouse()) xe::InputSystem::SetCaptureMouse(false);
			xe::InputSystem::GetMousePos(&mousePos.x);
			std::cout << "Position relative to Window: " << mousePos.x << ", " << mousePos.y << std::endl;
			break;

			// 3 - Get Mouse Position in Screen Space
		case 3:
			if (xe::InputSystem::GetCaptureMouse()) xe::InputSystem::SetCaptureMouse(false);
			xe::InputSystem::GetMousePos(&mousePos.x, false);
			std::cout << "Position in Screen: " << mousePos.x << ", " << mousePos.y << std::endl;
			break;

			// 4 - Mouse Delta & Capture Mouse
		case 4:
			if (!xe::InputSystem::GetCaptureMouse()) xe::InputSystem::SetCaptureMouse(true);
			xe::InputSystem::GetMouseDelta(&mousePos.x);
			std::cout << "Mouse Delta: " << mousePos.x << ", " << mousePos.y << std::endl;
			break;
		}
	}
	window.Terminate();

	return 0;
}