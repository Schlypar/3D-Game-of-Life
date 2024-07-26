#pragma once

#include "precompiled.h"

#include <sstream>
#include <string>

namespace GoL {

#define EVENT_IMPL_GETTYPE(type)                        \
    static Event::Type GetStaticType() {                \
        return Event::Type::type;                       \
    }                                                   \
    virtual Event::Type GetEventType() const override { \
        return GetStaticType();                         \
    }                                                   \
    virtual const char* GetName() const override {      \
        return #type;                                   \
    }

#define EVENT_IMPL_GETCATEGORY(category)            \
    virtual int GetCategoryFlags() const override { \
        return category;                            \
    }

// Will be used for binding methods to be handlers for events
#define BIND_MEMBER_EVENT_FN(fn)                          \
    [this](auto&&... args) -> decltype(auto) {            \
        return fn(std::forward<decltype(args)>(args)...); \
    }

class Event {
public:
    enum class Type {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    // bitfield
    enum Category {
        None = 0,
        Application = 1 << 0,
        Input = 1 << 1,
        Keyboard = 1 << 2,
        Mouse = 1 << 3,
        MouseButton = 1 << 4
    };

public:
    bool Handled = false;

    virtual ~Event() = default;

    /**
    * @name GetEventType - Returns type of event
    * @return Type
    */
    virtual Type GetEventType() const = 0;

    /**
    * @name GetName - Returns name of the type
    * @return char*
    */
    virtual const char* GetName() const = 0;

    /**
    * @name GetCategoryFlags - Returns all category flags
    * @return int - Bitfield of category flags
    */
    virtual int GetCategoryFlags() const = 0;

    /**
    * @name ToString - Returns name of the type
    * @return std::string
    */
    virtual std::string ToString() const {
        return GetName();
    }

    /**
    * @name IsInCategory - Checks if Event is in category
    * @param category -  Category to be checked
    * @return bool
    */
    bool IsInCategory(Category category) {
        return GetCategoryFlags() & category;
    }
};

class EventDispatcher {
    template <typename T>
    using EventFunc = std::function<bool(T&)>;

private:
    Event& event;

public:
    EventDispatcher(Event& event)
        : event(event) {
    }

    template <typename T, typename F>
    bool Dispatch(const F& func) {
        if (event.GetEventType() == T::GetStaticType()) {
            event.Handled |= func(static_cast<T&>(event));
            return true;
        }
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
    return os << e.ToString();
}

}
