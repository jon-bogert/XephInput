#include <iostream>

#include "XephInput/InputSystem.h"
#include "Window.h"

/*
* Demo Scene showing basic button input
* 
* Showing Devices:
*	- Gamepad
*   - Gamepad axis as button
*	- Multiplayer Gamepad
*   - Keyboard
*	- Mouse Buttons
*
* Shows getting Button Down and Up events as well as current press state (Hold)
*/

int main(int argc, char* argv[])
{
	//Initialize Demo Window
	Window window;
	window.Initialize(GetModuleHandle(nullptr), L"Window", 500, 500);
	HWND hwnd = window.GetWindowHandle();

	//Initialize InputSystem with demo window;
	xe::InputSystem::Initialize(hwnd);

	std::cout << "Press ESC at any time to quit..." << std::endl;
	std::cout << "Press M to toggle between Event Mode and Hold Mode" << std::endl << std::endl;
	bool eventMode = true;

	//Application Loop
	while (true)
	{
		xe::InputSystem::Update(); //Update all inputs this frame

		if (xe::InputSystem::GetKeyDown(xe::Key::Esc)) // Close window if ESC is pressed
			break;
		if (xe::InputSystem::GetKeyDown(xe::Key::M))
		{
			eventMode = !eventMode;
			if (eventMode)
				std::cout << "Event Mode active..." << std::endl;
			else
				std::cout << "Hold Mode active..." << std::endl;
		}
		if (eventMode)
		{
			//Down Events
			if (xe::InputSystem::GetGamepadDown(xe::Gamepad::Button::A)) // Gamepad
			{
				std::cout << "Player 1 Gamepad A was pressed" << std::endl;
			}
			if (xe::InputSystem::GetGamepadDown(xe::Gamepad::Button::A, 1)) // Gamepad player 2
			{
				std::cout << "Player 2 Gamepad A was pressed" << std::endl;
			}
			if (xe::InputSystem::GetKeyDown(xe::Key::Space)) // Keyboard
			{
				std::cout << "Space key was pressed" << std::endl;
			}
			if (xe::InputSystem::GetMouseDown(xe::Mouse::Button::Left)) // Mouse
			{
				std::cout << "Mouse left button was pressed" << std::endl;
			}
			if (xe::InputSystem::GetGamepadDown(xe::Gamepad::Button::LS_Up)) // Gamepad Axis (non-button)
			{
				std::cout << "Gamepad left stick up was pressed" << std::endl;
			}

			//Up Events
			if (xe::InputSystem::GetGamepadUp(xe::Gamepad::Button::A)) // Gamepad
			{
				std::cout << "Player 1 Gamepad A was released" << std::endl;
			}
			if (xe::InputSystem::GetGamepadUp(xe::Gamepad::Button::A, 1)) // Gamepad player 2pad
			{
				std::cout << "Player 2 Gamepad A was released" << std::endl;
			}
			if (xe::InputSystem::GetKeyUp(xe::Key::Space)) // Keyboard
			{
				std::cout << "Space key was release" << std::endl;
			}
			if (xe::InputSystem::GetMouseUp(xe::Mouse::Button::Left)) // Mouse
			{
				std::cout << "Mouse left button was released" << std::endl;
			}
			if (xe::InputSystem::GetGamepadUp(xe::Gamepad::Button::LS_Up)) // Gamepad Axis (non-button)
			{
				std::cout << "Gamepad left stick up was released" << std::endl;
			}
		}
		else //!eventMode
		{
			//Hold
			if (xe::InputSystem::GetGamepadHold(xe::Gamepad::Button::A)) // Gamepad
			{
				std::cout << "Player 1 Gamepad A is currently pressed" << std::endl;
			}
			if (xe::InputSystem::GetGamepadHold(xe::Gamepad::Button::A, 1)) // Gamepad player 2
			{
				std::cout << "Player 2 Gamepad A is currently pressed" << std::endl;
			}
			if (xe::InputSystem::GetKeyHold(xe::Key::Space)) // Keyboard
			{
				std::cout << "Space key is currently pressed" << std::endl;
			}
			if (xe::InputSystem::GetMouseHold(xe::Mouse::Button::Left)) // Mouse
			{
				std::cout << "Mouse left button is currently pressed" << std::endl;
			}
			if (xe::InputSystem::GetGamepadHold(xe::Gamepad::Button::LS_Up)) // Gamepad Axis (non-button)
			{
				std::cout << "Gamepad left stick up is currently pressed" << std::endl;
			}
		}
	}
	window.Terminate();

	return 0;
}