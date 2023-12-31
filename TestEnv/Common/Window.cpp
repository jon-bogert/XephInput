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

#include "Window.h"
#include <algorithm>

LRESULT CALLBACK WindowMessageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

void Window::Initialize(HINSTANCE instance, std::wstring appName, uint32_t width, uint32_t height)
{
	_instance = instance;
	_appName = appName;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowMessageHandler;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // Standard icon
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _appName.c_str();
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = (static_cast<int>(rc.right - rc.left) < screenWidth) ? static_cast<int>(rc.right - rc.left) : screenWidth;
	const int winHeight = (static_cast<int>(rc.bottom - rc.top) < screenHeight) ? static_cast<int>(rc.bottom - rc.top) : screenHeight;
	const int left = (screenWidth - winWidth) / 2;
	const int top = (screenHeight - winHeight) / 2;

	_window = CreateWindow
	(
		_appName.c_str(),
		_appName.c_str(),
		WS_OVERLAPPEDWINDOW,
		left, top,
		winWidth, winHeight,
		nullptr, nullptr,
		instance, nullptr
	);

	ShowWindow(_window, SW_SHOWNORMAL);
	SetCursorPos(screenWidth / 2, screenHeight / 2);

	_isActive = (_window);

}

void Window::Terminate()
{
	DestroyWindow(_window);

	UnregisterClass(_appName.c_str(), _instance);

	_window = nullptr;
	_instance = nullptr;
}

void Window::ProcessMessage()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			_isActive = false;
		}
	}
}