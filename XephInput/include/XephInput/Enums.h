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

#ifndef __XEPHINPUT_ENUMS_H__
#define __XEPHINPUT_ENUMS_H__

namespace xe
{
    enum class Key //will typecast to VK_ macros
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
    class Mouse //will typecast to VK_ macros
    {
    public:
        Mouse() = delete;
        enum class Button
        {
            Left = 1, Right, Middle = 4, Button4, Button5
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
}

#endif //!__XEPHINPUT_ENUMS_H__