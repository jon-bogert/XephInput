#pragma once
#include "InputSystem.h"
#include <functional>
#include <map>
#include <vector>

#define XEInputActionCallback(function) static_cast<void*>(this), std::bind(&function, this, std::placeholders::_1)
#define XEInputActionCallbackPtr(function, ptr) static_cast<void*>(ptr), std::bind(&function, ptr, std::placeholders::_1)

namespace xe
{
    class InputAction;
    class InputActionMap;

    class InputActionEvent
    {
        friend class InputAction;
    private:

        struct EventEntry
        {
            void* object;
            std::function<void(InputAction*)> function;
        };

        std::list<EventEntry> _events;

    public:
        void Subscribe(void* object, std::function<void(InputAction*)> function)
        {
            _events.push_back({ object, function });
        }

        void UnsubscribeAll(void* object)
        {
            _events.remove_if([=](const EventEntry& x) {return object == x.object; });
        }

    private:
        void Invoke(InputAction* agent) {
            for (const auto& event : _events)
            {
                event.function(agent);
            }
        }
    };

    class InputAction
    {
        friend class InputActionMap;
    public:
        enum class Type { Button, Axis1D, Axis2D };
        enum class ButtonEvent { Down, Up, DownUp };

        std::string name = "";
        InputActionEvent performed;

    private:
        Type _type = Type::Button;
        ButtonEvent _buttonEvent = ButtonEvent::Down;

        InputActionMap* _map = nullptr;
        void* _data;
        bool _triggered = false;

    public:
        InputAction(std::string name = "", Type type = Type::Button, ButtonEvent buttonEvent = ButtonEvent::Down);
        virtual ~InputAction();

        Type GetType() const; 
        void AddButton(Gamepad::Button button, uint8_t player = 0);
        void AddButton(Key key);
        void Add1DAxis(Gamepad::Axis axis, uint8_t component, uint8_t player = 0);
        void Add2DAxis(Gamepad::Axis axis, uint8_t player = 0);

        //TODO ADD Composites
        //TODO Remove Functions

        void ReadValue(void* out);

    private:
        void RaiseEvent();
        
    };

    class InputActionMap
    {
        friend class InputAction;

        std::list<std::unique_ptr<InputAction>> _inputActions;

        std::map<Gamepad::Button, std::pair<bool, std::vector<InputAction*>>> _buttonActions;
        std::map<Key, std::pair<bool, std::vector<InputAction*>>> _keyActions;
        std::map<std::pair<Gamepad::Axis, uint8_t>, std::pair<float, std::vector<InputAction*>>> _1DActions;
        std::map<Gamepad::Axis, std::pair<float[2], std::vector<InputAction*>>> _2DActions;

    public:
        InputAction* CreateAction(std::string name, InputAction::Type type = InputAction::Type::Button, InputAction::ButtonEvent buttonEvent = InputAction::ButtonEvent::Down);
        void Update();
    };
}