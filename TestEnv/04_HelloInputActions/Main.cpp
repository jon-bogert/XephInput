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
* Demo Scene showing how to use InputAction's
* 
* InputAction's are an easy way to have multiple bindings (Gamepad or M&K) as well as triggering events instead of checking data on Update loop
* Checking data on Update is possible by storing a pointer to the InputAction and calling "inputAction->ReadValue(&value);" on your Update loop
*/

//Custom standard struct class (or from any library)
struct Vector2
{
	float x;
	float y;
};

class TestObject
{
	std::string myStr = "Button Pressed";
	Vector2 moveVector;
	float modifier = 0.f;

private:
	//Callbacks must have signature: "void(xe::InputActionCallback*)"
	void PrintString(xe::InputAction* context)
	{
		std::cout << myStr << std::endl;
	}
	void Move(xe::InputAction* context)
	{
		context->ReadValue(&moveVector.x);
		std::cout << "Move Vector: " << moveVector.x << ", " << moveVector.y << std::endl;
	}
	void Modifier(xe::InputAction* context)
	{
		context->ReadValue(&modifier);
		std::cout << "Modifier: " << modifier << std::endl;
	}
public:
	void Start()
	{
		//Subscribe private functions to be called by InputAction Events
		xe::InputActionMap* map = xe::InputSystem::FindInputActionMap("Gameplay");
		map->FindInputAction("ButtonPress")->performed.Subscribe(XEInputActionCallback(TestObject::PrintString));
		map->FindInputAction("2D Axis")->performed.Subscribe(XEInputActionCallback(TestObject::Move));
		map->FindInputAction("1D Axis")->performed.Subscribe(XEInputActionCallback(TestObject::Modifier));
		//Note: can use XEInputActionCallbackPtr(function, ptr) for specifying a pointer to an object that is not "this"
	}

	void Destroy()
	{
		//Unubscribe private functions to be called by InputAction Events
		xe::InputActionMap* map = xe::InputSystem::FindInputActionMap("Gameplay");
		map->FindInputAction("ButtonPress")->performed.UnsubscribeAll(this);
		map->FindInputAction("2D Axis")->performed.UnsubscribeAll(this);
		map->FindInputAction("1D Axis")->performed.UnsubscribeAll(this);
	}
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

	//Setup InputAction's
	xe::InputActionMap* gameplayMap = xe::InputSystem::CreateInputActionMap("Gameplay"); // Create and get reference to new InputActionMap

	//Create and assign bindings to button press
	xe::InputAction* buttonPress = gameplayMap->CreateAction("ButtonPress", xe::InputAction::Type::Button, xe::InputAction::ButtonEvent::Down); // Can chose Down, Up or DownUp
	buttonPress->AddButton(xe::Gamepad::Button::A);		// Adding Gamepad button A
	buttonPress->AddButton(xe::Key::Space);				// Adding Key Space
	buttonPress->AddButton(xe::Mouse::Button::Left);	// Adding Mouse Button Left

	//Create and Assign bindings to 2D Axis
	xe::InputAction* axis2D = gameplayMap->CreateAction("2D Axis", xe::InputAction::Type::Axis2D);
	axis2D->Add2DAxis(xe::Gamepad::Axis::LS);												// Adding Gamepad Left Stick
	axis2D->Add2DAxis(xe::Key::A, xe::Key::D, xe::Key::S, xe::Key::W);	// Adding WASD Key composite;

	//Create and Assign bindings to 1D Axis
	xe::InputAction* axis1D = gameplayMap->CreateAction("1D Axis", xe::InputAction::Type::Axis1D);
	axis1D->Add1DAxis(xe::Gamepad::Axis::RS, 0);	// Adding Gamepad Right Stick X-Component
	axis1D->Add1DAxis(xe::Key::Z, xe::Key::X);		// Adding ZX Key composite;		

	TestObject testObj; //Object resposible for Events
	testObj.Start(); // See TestObject::Start for Subscribe info

	//Application Loop
	while (true)
	{
		xe::InputSystem::Update(); //Update all inputs this frame

		if (xe::InputSystem::GetKeyDown(xe::Key::Esc)) // Close window if ESC is pressed
			break;
	}
	testObj.Destroy(); // See TestObject::Destroy for Unsubscribe info
	window.Terminate();

	return 0;
}