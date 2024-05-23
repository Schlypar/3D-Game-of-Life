#pragma once

#include <sstream>

#include "Event.h"

namespace GoL {

class MouseMovedEvent : public Event {
private:
    float mouseX, mouseY;

public:
    MouseMovedEvent(const float x, const float y)
        : mouseX(x)
        , mouseY(y) {
    }

    float GetX() const {
        return mouseX;
    }

    float GetY() const {
        return mouseY;
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(MouseMoved)
    EVENT_IMPL_GETCATEGORY(Event::Category::Mouse | Event::Category::Input)
};

class MouseScrolledEvent : public Event {
private:
    float xOffset, yOffset;

public:
    MouseScrolledEvent(const float xOffset, const float yOffset)
        : xOffset(xOffset)
        , yOffset(yOffset) {
    }

    float GetXOffset() const {
        return xOffset;
    }

    float GetYOffset() const {
        return yOffset;
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(MouseScrolled)
    EVENT_IMPL_GETCATEGORY(Event::Category::Mouse | Event::Category::Input)
};

class MouseButtonEvent : public Event {
protected:
    using MouseCode = unsigned int;

    MouseButtonEvent(const MouseCode button)
        : button(button) {
    }

    MouseCode button;

public:
    MouseCode GetMouseButton() const {
        return button;
    }

    EVENT_IMPL_GETCATEGORY(Event::Category::Mouse | Event::Category::Input | Event::Category::MouseButton)
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(const MouseCode button)
        : MouseButtonEvent(button) {
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << button;
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(const MouseCode button)
        : MouseButtonEvent(button) {
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << button;
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(MouseButtonReleased)
};

}