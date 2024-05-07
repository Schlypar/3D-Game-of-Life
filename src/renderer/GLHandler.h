#pragma once

#include <concepts>
#include "GLConfiguration.h"

namespace GoL {

template <typename H>
concept Handler = requires(H handler) {
    { H() };
    { handler.Config() } -> std::convertible_to<Configuration*>;
};

};