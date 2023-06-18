#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#pragma comment(lib, "XInput.lib")

#include "InputSystem.h"
#include "InputActionMap.h"

std::bitset<256> store;

int main()
{
	while (true)
	{
		xe::InputSystem::Update();

		if (xe::InputSystem::GetMouseDown(xe::Mouse::Button::Button5))
		{
			std::cout << "BAM" << std::endl;
		}
		if (xe::InputSystem::GetKeyDown(xe::Key::Esc))
			break;
	}

	return 0;
}