#pragma once

#include <ostream>

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;

    Vertex(const glm::vec3& position, const glm::vec3& color)
        : position(position)
        , color(color) {
    }

    Vertex(const Vertex& other)
        : position(other.position)
        , color(other.color) {
    }

    Vertex& operator=(const Vertex& other) {
        if (this == &other) {
            return *this;
        }

        this->color = other.color;
        this->position = other.position;

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, Vertex& v) {
        os << "Pos: "
           << "(x: " << v.position.x
           << ", y: " << v.position.y
           << ", z: " << v.position.z << ")";
        return os;
    }
};
