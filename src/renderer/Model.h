#pragma once

#include <concepts>
#include <glm/glm.hpp>

#include "VertexBufferLayout.h"
#include "Models/Vertex.h"

namespace GoL {

template <typename T>
concept Model = requires(T m, T n) {
    { T() };
    { m = n } -> std::convertible_to<T>;
    { m.Draw() };
    { m.BindIndices() };
    { m.GetModelMatrix() } -> std::convertible_to<glm::mat4>;
    { m.GetVerticies() } -> std::convertible_to<std::pair<const Vertex*, int>>;
};

}