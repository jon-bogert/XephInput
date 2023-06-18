#pragma once
#include <Windows.h>
#include <string>
namespace xe
{
	namespace Core
	{
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
	}
}

