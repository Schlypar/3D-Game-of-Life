#pragma once

#include <sstream>

#include "Event.h"

namespace GoL {

class WindowResizeEvent : public Event {
private:
    unsigned int width, height;

public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : width(width)
        , height(height) {
    }

    unsigned int GetWidth() const {
        return width;
    }

    unsigned int GetHeight() const {
        return height;
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << width << ", " << height;
        return ss.str();
    }

    EVENT_IMPL_GETTYPE(WindowResize)
    EVENT_IMPL_GETCATEGORY(Event::Category::Application);
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;

    EVENT_IMPL_GETTYPE(WindowClose)
    EVENT_IMPL_GETCATEGORY(Event::Category::Application)
};


}