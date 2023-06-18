#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <list>

namespace xe
{
    enum class Key
    {
        Backspace = 8, Tab, Enter = 13, Shift = 16, Ctrl, Alt, Pause, CapsLock,
        Esc = 27, Space = 32, PageUp, PageDown, End, Home, Left, Up, Right, Down, PrintScreen = 44, Insert, Delete,
        Zero = 48, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LWin, RWin, 
        Numpad0 = 96, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9, NumpadMult, NumpadPlus, NumpadMinus = 109, NumpadDecimal, NumpadDivide,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        LShift = 160, RShift, LCtrl, RCtrl, LAlt, RAlt
    };
    class Mouse
    {
    public:
        Mouse() = delete;
        enum class Button
        {
            Left, Right, Middle, Button4, Button5
        };
    };

    class Gamepad final
    {
    public:
        Gamepad() = delete;
        enum class Button
        {
            A, B, X, Y, LB, RB, Select, Start, LS_Press, RS_Press, LT, RT,
            LS_Up, LS_Down, LS_Left, LS_Right,
            RS_Up, RS_Down, RS_Left, RS_Right,
            DPad_Up, DPad_Down, DPad_Left, DPad_Right
        };
        enum class Axis
        {
            LS, RS, DPad, Trig
        };
    };

    class KeyHandler;
	class InputSystem final
	{
	private:
        struct Axis
        {
            float lx = 0.f;
            float ly = 0.f;
            float rx = 0.f;
            float ry = 0.f;
            float lt = 0.f;
            float rt = 0.f;
        };

        XINPUT_STATE _state{};
		HWND* _hwnd = nullptr;

        KeyHandler* _keyHandler = nullptr;

		bool _controllerActive = true;
		uint8_t  _controllerCount = 0;
        bool _controllerCountChange = false;

        WORD _controllerButtonHold[XUSER_MAX_COUNT];
        WORD _controllerButtonDown[XUSER_MAX_COUNT];
        WORD _controllerButtonUp[XUSER_MAX_COUNT];
        WORD _controllerAxisHold[XUSER_MAX_COUNT];
        WORD _controllerAxisDown[XUSER_MAX_COUNT];
        WORD _controllerAxisUp[XUSER_MAX_COUNT];
        Axis _controllerAxisState[XUSER_MAX_COUNT];

        float _triggerThreshold = 0.1f;
        float _deadzoneMinimum = 0.3f;
        float _deadzoneMaximum = 1.f;

        InputSystem();
		static InputSystem& Get();
	public:

        ~InputSystem();

		static void Initialize(HWND& hwnd, const bool controllerActive = true);
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

	private:
		// Public impl
		void _Initialize(HWND& hwnd, const bool controllerActive);
		void _Update();

        bool _IsControllerConnected(const uint8_t num) const;
        bool _GetGamepadHold(Gamepad::Button button, uint8_t num = 0);
        bool _GetGamepadDown(Gamepad::Button button, uint8_t num = 0);
        bool _GetGamepadUp(Gamepad::Button button, uint8_t num = 0);
        void _GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num = 0);


        float _GetTriggerThreshold() const;
        void _SetTriggerThreshold(const float threshold);

        void UpdateAxisState(uint8_t index);

	};
}

