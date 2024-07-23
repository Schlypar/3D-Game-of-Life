#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/transform.hpp>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "Logger.h"
