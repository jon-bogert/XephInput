#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <list>

namespace xe
{
    enum class Key
    {
        Unkwn = -1,
        A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
        Escape, LCtrl, LShift, LAlt, LWin, RCtrl, RShift, RAlt, RWin,
        Menu, LBracket, RBracket, Semicolon, Comma, Period, Quote, ForwardSlash, Backslash,
        Tilde, Equal, Dash, Space, Enter, Backspace, Tab,
        PageUp, PageDown, End, Home, Insert, Delete,
        NumAdd, NumMinus, NumMultiply, NumDivide,
        Left, Right, Up, Down,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
        Pause
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

		InputSystem() {}
		static InputSystem& Get();
	public:

		static void Initialize(HWND& hwnd, const bool controllerActive = true);
		static void Update();

        static bool IsControllerConnected(const uint8_t num);
        static bool GetGamepadHold(Gamepad::Button button, uint8_t num = 0);
        static bool GetGamepadDown(Gamepad::Button button, uint8_t num = 0);
        static bool GetGamepadUp(Gamepad::Button button, uint8_t num = 0);
        static void GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num = 0);

        static float GetTriggerThreshold();
        static void SetTriggerThreshold(const float threshold);

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

