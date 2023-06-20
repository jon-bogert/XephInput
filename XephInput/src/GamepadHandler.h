#pragma once
#include "XephInput/InputSystem.h"
namespace xe
{
	class GamepadHandler
	{
		friend class InputSystem;

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

		void Initialize();
		void Update();

		bool IsControllerConnected(const uint8_t num);
		bool GetGamepadHold(Gamepad::Button button, uint8_t num = 0);
		bool GetGamepadDown(Gamepad::Button button, uint8_t num = 0);
		bool GetGamepadUp(Gamepad::Button button, uint8_t num = 0);
		void GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num = 0);

		float GetTriggerThreshold();
		void SetTriggerThreshold(const float threshold);

		void UpdateAxisState(uint8_t index);
	};
}

