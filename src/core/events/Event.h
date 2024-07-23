#pragma once

#include <sstream>
#include <string>

#include <functional>

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
    virtual ~Event() = default;

    bool Handled = false;

    virtual Type GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;

    virtual std::string ToString() const {
        return GetName();
    }

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
