#pragma once

#include "events/Event.h"

namespace GoL {

class Layer {
protected:
    std::string label;

public:
    Layer(const std::string& name = "Not defined");
    virtual ~Layer() = default;

    virtual void OnAttach() {
    }

    virtual void OnDetach() {
    }

    virtual void OnUpdate() {
    }

    virtual void OnEvent(Event* event) {
    }

    const std::string& GetName() const {
        return label;
    }
};

}