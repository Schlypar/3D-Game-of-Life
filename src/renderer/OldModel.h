#pragma once

#include <concepts>
#include <glm/glm.hpp>

namespace GoL {

template <typename T>
concept OldModel = requires(T m) {
    { m.Draw() };
    { m.BindIndices() };
    { m.GetModelMatrix() } -> std::convertible_to<glm::mat4>;
};

}