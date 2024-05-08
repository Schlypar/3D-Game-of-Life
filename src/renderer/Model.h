#pragma once

#include <concepts>
#include <glm/glm.hpp>

#include "Models/Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

template <typename T>
concept Model = requires(T m, T n) {
    { T() };
    { m = n } -> std::convertible_to<T>;
    { m.Draw() };
    { m.BindIndices() };
    { m.GetModelMatrix() } -> std::convertible_to<glm::mat4>;
    { m.GetVAO() } -> std::convertible_to<VertexArray&>;
    { m.GetVBO() } -> std::convertible_to<VertexBuffer&>;
    { m.GetVerticies() } -> std::convertible_to<std::pair<const Vertex*, int>>;
};

}