#pragma once
#include <functional>
#include <list>

#define XEEventCallback(function) static_cast<void*>(this), std::bind(&function, this)
#define XEEventCallbackPtr(function, ptr) static_cast<void*>(ptr), std::bind(&function, ptr)

namespace xe
{
    class Event
    {
    private:
        struct EventEntry
        {
            void* object;
            std::function<void(void)> function;
        };

        std::list<EventEntry> events;

    public:
        void Subscribe(void* object, std::function<void(void)> function)
        {
            events.push_back({ object, function });
        }

        void UnsubscribeAll(void* object)
        {
            events.remove_if([=](const EventEntry& x) {return object == x.object; });
        }

        void Invoke() {
            for (const auto& event : events)
            {
                event.function();
            }
        }
    };
}

