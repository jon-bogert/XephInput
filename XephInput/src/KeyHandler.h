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

#ifndef __XEPHINPUT_KEYHANDLER_H__
#define __XEPHINPUT_KEYHANDLER_H__

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

#endif // !__XEPHINPUT_KEYHANDLER_H__