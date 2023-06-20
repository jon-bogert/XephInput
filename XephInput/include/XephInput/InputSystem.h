#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <list>

#include <XephInput/Enums.h>
#include <XephInput/InputActionMap.h>

namespace xe
{

    class GamepadHandler;
    class KeyHandler;
	class InputSystem final
	{
	private:
		HWND _hwnd = nullptr;

        GamepadHandler* _gamepadHandler = nullptr;
        KeyHandler* _keyHandler = nullptr;

        POINT _mousePos;
        float _mouseDelta[2] = { 0.f, 0.f };
        bool _captureMouse = false;

        std::list<std::unique_ptr<InputActionMap>> _actionMaps;

        InputSystem();
		static InputSystem& Get();
	public:

        ~InputSystem();

		static void Initialize(HWND& hwnd);
		static void Update();

        static bool IsControllerConnected(const uint8_t num);
        static bool GetGamepadHold(Gamepad::Button button, uint8_t num = 0);
        static bool GetGamepadDown(Gamepad::Button button, uint8_t num = 0);
        static bool GetGamepadUp(Gamepad::Button button, uint8_t num = 0);
        static void GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num = 0);

        static float GetTriggerThreshold();
        static void SetTriggerThreshold(const float threshold);

        static bool GetKeyHold(Key keycode);
        static bool GetKeyDown(Key keycode);
        static bool GetKeyUp(Key keycode);
        static bool GetMouseHold(Mouse::Button btncode);
        static bool GetMouseDown(Mouse::Button btncode);
        static bool GetMouseUp(Mouse::Button btncode);
        static float GetKeyAxisComposite1D(Key negative, Key positive);
        static void GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY);

        static void GetMousePos(float* out_v2, bool relativeToWindow = true);
        static void GetMouseDelta(float* out_v2);

        static void SetCaptureMouse(const bool captureMouse);
        static bool GetCaptureMouse();
        static bool MouseOverWindow();

        //Creates a new InputActionMap and returns a reference pointer to it
        static InputActionMap* CreateInputActionMap(std::string name);
        //returns reference to an Input Action Map by name
        static InputActionMap* FindInputActionMap(std::string name);

	private:
		// Public impl
		void _Initialize(HWND& hwnd);
		void _Update();

        void _GetMousePos(float* out_v2, bool relativeToWindow);

	};
}

