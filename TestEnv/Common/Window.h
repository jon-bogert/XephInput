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

#ifndef __XEPHINPUT_DEMO_WINDOW_H__
#define __XEPHINPUT_DEMO_WINDOW_H__

#include <Windows.h>
#include <string>

class Window
{
	HINSTANCE _instance = nullptr;
	HWND _window = nullptr;
	std::wstring _appName;
	bool _isActive = false;

public:
	void Initialize(HINSTANCE instance, std::wstring appName, uint32_t width, uint32_t height);
	void Terminate();

	void ProcessMessage();

	HWND GetWindowHandle() const { return _window; }
	bool IsActive() const { return _isActive; }
};

#endif // !__XEPHINPUT_DEMO_WINDOW_H__