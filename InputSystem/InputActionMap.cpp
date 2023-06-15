#include "InputActionMap.h"

xe::InputAction::InputAction(std::string name, Type type, ButtonEvent buttonEvent)
	: name(name), _type(type), _buttonEvent(buttonEvent)
{
	switch (type)
	{
	case Type::Button:
		_data = static_cast<void*>(new bool);
		break;
	case Type::Axis1D:
		_data = static_cast<void*>(new float);
		break;
	case Type::Axis2D:
		_data = static_cast<void*>(new float[2]);
	}
}

xe::InputAction::~InputAction()
{
	switch (_type)
	{
	case Type::Button:
		delete static_cast<bool*>(_data);
		break;
	case Type::Axis1D:
		delete static_cast<float*>(_data);
		break;
	case Type::Axis2D:
		delete[] static_cast<float*>(_data);
	}
}

xe::InputAction::Type xe::InputAction::GetType() const
{
	return _type;
}

void xe::InputAction::AddButton(Gamepad::Button button, uint8_t player)
{
	if (_type != Type::Button) return; // return if not of type
	auto it = _map->_buttonActions.find(button); // try to find existing button
	if (it == _map->_buttonActions.end()) // was not found, create one
	{
		_map->_buttonActions.insert({ button, {} });
		it = _map->_buttonActions.find(button);
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void xe::InputAction::Add1DAxis(Gamepad::Axis axis, uint8_t component, uint8_t player)
{
	if (_type != Type::Axis1D) return;
	auto it = _map->_1DActions.find({ axis, component }); // try to find existing input
	if (it == _map->_1DActions.end()) // was not found, create one
	{
		_map->_1DActions.insert({ {axis, component}, {} });
		it = _map->_1DActions.find({ axis, component });
	}
	//return if this InputAction is already assigned to this input
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void xe::InputAction::Add2DAxis(Gamepad::Axis axis, uint8_t player)
{
	if (_type != Type::Axis2D) return;
	auto it = _map->_2DActions.find(axis); // try to find existing button
	if (it == _map->_2DActions.end()) // was not found, create one
	{
		_map->_2DActions.insert({ axis, {} });
		it = _map->_2DActions.find(axis);
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void xe::InputAction::ReadValue(void* out)
{
	switch (_type)
	{
	case Type::Button:
		*static_cast<bool*>(out) = *static_cast<bool*>(_data);
		return;
	case Type::Axis1D:
		*static_cast<float*>(out) = *static_cast<float*>(_data);
		return;
	case Type::Axis2D:
		*static_cast<float*>(out) = *static_cast<float*>(_data);
		*(static_cast<float*>(out) + 1) = *(static_cast<float*>(_data) + 1);
		return;
	default:
		return;
	}
}

void xe::InputAction::RaiseEvent()
{
	performed.Invoke(this);
}

xe::InputAction* xe::InputActionMap::CreateAction(std::string name, InputAction::Type type, InputAction::ButtonEvent buttonEvent)
{
	 _inputActions.emplace_back(std::make_unique<InputAction>(name, type, buttonEvent));
	 InputAction* ptr = _inputActions.back().get();
	 ptr->_map = this;
	 return ptr;
}

void xe::InputActionMap::Update()
{
	for (auto& action : _inputActions) action->_triggered = false;

	{ // BUTTONS       [ Axis ]     [ oldVal | vector<pointers> ]
		bool both;
		bool down;
		bool up;
		bool hold;
		for (auto& button : _buttonActions)
		{
			hold = InputSystem::GetGamepadHold(button.first);
			down = InputSystem::GetGamepadDown(button.first);
			up = InputSystem::GetGamepadUp(button.first);
			both = down || up;
			for (auto& action : button.second.second)
			{
				if (button.second.first != hold)
				{
					button.second.first = hold;
					if (action->_triggered) continue;
					action->_triggered = true;
					*static_cast<bool*>(action->_data) = hold;
					if (action->_buttonEvent == InputAction::ButtonEvent::Down && down)
					{
						action->RaiseEvent();
					}
					else if (action->_buttonEvent == InputAction::ButtonEvent::Up && up)
					{
						action->RaiseEvent();
					}
					else if (action->_buttonEvent == InputAction::ButtonEvent::DownUp && both)
					{
						action->RaiseEvent();
					}
				}
			}
		}
	}
	{ // FLOAT       [ Axis | Dimension ]     [ oldVal | vector<pointers> ]
		float oldVal = 0.f;
		float newVal = 0.f;
		float v2[2] = { 0.f, 0.f };
		for (auto& axis : _1DActions)
		{
			oldVal = axis.second.first;
			InputSystem::GetGamepadAxis(&v2[0], axis.first.first);
			newVal = v2[axis.first.second];
			for (auto& action : axis.second.second)
			{
				if (oldVal != newVal)
				{
					axis.second.first = oldVal;
					if (action->_triggered) continue;
					action->_triggered = true;
					*static_cast<float*>(action->_data) = newVal;
					action->RaiseEvent();
				}
			}
		}
	}
	{ // VEC2       [ Axis ]     [ oldVal | vector<pointers> ]
		float oldVal[2] = { 0.f, 0.f };
		float newVal[2] = { 0.f, 0.f };
		for (auto& axis : _2DActions)
		{
			oldVal[0] = axis.second.first[0];
			oldVal[1] = axis.second.first[1];
			InputSystem::GetGamepadAxis(&newVal[0], axis.first);
			for (auto& action : axis.second.second)
			{
				if (oldVal[0] != newVal[0] || oldVal[1] != newVal[1])
				{
					axis.second.first[0] = newVal[0];
					axis.second.first[1] = newVal[1];
					if (action->_triggered) continue;
					action->_triggered = true;
					*static_cast<float*>(action->_data) = newVal[0];
					*(static_cast<float*>(action->_data) + 1) = newVal[1];
					action->RaiseEvent();
				}
			}
		}
	}
}
