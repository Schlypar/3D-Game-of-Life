#pragma once

#include "Event.h"
#include <sstream>

namespace GoL {

class KeyEvent : public Event {
protected:
    using KeyCode = unsigned int;

    KeyEvent(const KeyCode keycode)
        : keyCode(keycode) {
    }

    KeyCode keyCode;

public:
    KeyCode GetKeyCode() const {
        return keyCode;
    }

    EVENT_IMPL_GETCATEGORY(Event::Category::Keyboard | Event::Category::Input)
};

class KeyPressedEvent : public KeyEvent {
private:
    bool isRepeated;

public:
    KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
        : KeyEvent(keycode)
        , isRepeated(isRepeat) {
    }

    bool IsRepeat() const {
        return isRepeated;
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << keyCode << " (repeat = " << isRepeated << ")";
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(KeyPressed)
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(const KeyCode keycode)
        : KeyEvent(keycode) {
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << keyCode;
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(const KeyCode keycode)
        : KeyEvent(keycode) {
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << keyCode;
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(KeyTyped)
};

}