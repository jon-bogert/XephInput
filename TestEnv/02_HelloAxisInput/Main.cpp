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

//Print Functions for demo
void PrintVal(std::string tag, float what)
{
	std::cout << tag << ": " << what << std::endl;
}

void PrintVal(std::string tag, Vector2 what)
{
	std::cout << tag << ": " << what.x << ", " << what.y << std::endl;
}

int main(int argc, char* argv[])
{
	//Initialize Demo Window
	Window window;
	window.Initialize(GetModuleHandle(nullptr), L"Window", 500, 500);
	HWND hwnd = window.GetWindowHandle();

	//Initialize InputSystem with demo window;
	xe::InputSystem::Initialize(hwnd);

	std::cout << "Press ESC at any time to quit..." << std::endl;

	//declaring axis variables
	Vector2 leftStickP1;
	Vector2 leftStickP2;
	Vector2 triggersP1;
	Vector2 wasdComposite;
	
	float zxComposite;

	//Application Loop
	while (true)
	{
		xe::InputSystem::Update(); //Update all inputs this frame

		if (xe::InputSystem::GetKeyDown(xe::Key::Esc)) // Close window if ESC is pressed
			break;

		// == 2D AXIS ==
		//Axis from Gamepad (Left Stick)
		xe::InputSystem::GetGamepadAxis(&leftStickP1.x, xe::Gamepad::Axis::LS);
		if (leftStickP1.x != 0.f || leftStickP1.y != 0.f)
			PrintVal("P1 Left Stick", leftStickP1);

		//Axis from Gamepad Multiplayer (Left Stick Player 2)
		xe::InputSystem::GetGamepadAxis(&leftStickP2.x, xe::Gamepad::Axis::LS, 1);
		if (leftStickP2.x != 0.f || leftStickP2.y != 0.f)
			PrintVal("P2 Left Stick", leftStickP2);

		//Gamepad Triggers as Axis (X->Left Y->Right)
		xe::InputSystem::GetGamepadAxis(&triggersP1.x, xe::Gamepad::Axis::Trig);
		if (triggersP1.x != 0.f || triggersP1.y != 0.f)
			PrintVal("P1 Triggers (Left, Right)", triggersP1);

		//Getting 2D axis from Key Composite
		xe::InputSystem::GetKeyAxisComposite2D(&wasdComposite.x, xe::Key::A, xe::Key::D, xe::Key::S, xe::Key::W);
		if (wasdComposite.x != 0.f || wasdComposite.y != 0.f)
			PrintVal("WASD Key 2D Composite", wasdComposite);

		//Getting 1D axis from Key Composite
		zxComposite = xe::InputSystem::GetKeyAxisComposite1D(xe::Key::Z, xe::Key::X);
		if (zxComposite != 0.f)
			PrintVal("ZX Key 1D Composite", zxComposite);
	}
	window.Terminate();

	return 0;
}