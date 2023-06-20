#pragma once
#include <Windows.h>
#include <bitset>
#include <memory>

#include "XephInput/InputSystem.h"

namespace xe
{
	//class InputSystem;
	class KeyHandler
	{
		friend class InputSystem;
		std::bitset<256> _keyHold;
		std::bitset<256> _keyDown; 
		std::bitset<256> _keyUp;
		std::bitset<256> _keyRepeat;
		std::bitset<256> _keyBuffer;

		KeyHandler() {} // Only InputSystem can create one of these

		void Update();
		void PollKeys();

		bool GetKeyHold(Key keycode);
		bool GetKeyDown(Key keycode);
		bool GetKeyUp(Key keycode);

		bool GetMouseHold(Mouse::Button btncode);
		bool GetMouseDown(Mouse::Button btncode);
		bool GetMouseUp(Mouse::Button btncode);

		float GetKeyAxisComposite1D(Key negative, Key positive);
		void GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY);
	};
}

