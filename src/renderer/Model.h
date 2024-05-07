#pragma once

#include <glm/glm.hpp>
#include <concepts>

namespace GoL {

template <typename T>
concept Model = requires(T m, T n) {
    { T() };
    { m = n } -> std::convertible_to<T>;
    { m.Draw() };
    { m.BindIndices() };
    { m.GetModelMatrix() } -> std::convertible_to<glm::mat4>;
};

}