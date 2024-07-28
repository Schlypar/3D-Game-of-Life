#pragma once

#include "events/Event.h"

namespace GoL {

class Layer {
protected:
    std::string label;
    std::string parentSceneName;

public:
    Layer(const std::string& name = "Not defined");
    virtual ~Layer() = default;

    /**
    * @name OnAttach - Function called once to initialize some state to layer to work properly
    * @return void
    */
    virtual void OnAttach() {
    }

    /**
    * @name OnDetach - Function called once Layer is not needed to clean data and delete Layer
    * @return void
    */
    virtual void OnDetach() {
    }

    /**
    * @name OnUpdate - Called every frame to update state and behavoiur of Layer
    * @return void
    */
    virtual void OnUpdate() {
    }

    /**
    * @name OnEvent - Function that will handle events for this Layer
    * @param event -  Event to be handled
    * @return void
    */
    virtual void OnEvent(Event* event) {
    }

    /**
    * @name GetName - Gives name of Layer at hand
    * @return std::string - Name of Layer
    */
    inline const std::string& GetName() const {
        return label;
    }

    inline std::string& Scene() {
        return parentSceneName;
    }
};

}
