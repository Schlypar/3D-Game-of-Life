cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

project(cellular_automata)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# --- START PROJECT ---

file(GLOB source CONFIGURE_DEPENDS 
"${SRC_DIR}/*.cpp" 
"${SRC_DIR}/renderer/*.cpp" 
"${SRC_DIR}/core/*.cpp"
"${SRC_DIR}/core/events/*.cpp"
"${SRC_DIR}/core/layers/*.cpp"
)

# generate shader headers
add_executable(prebuild_setup ${SRC_DIR}/utils/PrebuildSetup.cpp)

# Build and execute prebuild_setup
add_custom_target(run_prebuild_setup
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --yellow "Building and executing prebuild_setup..."
    COMMAND ${CMAKE_BINARY_DIR}/prebuild_setup
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --blue "Finished building and executing prebuild_setup."
    DEPENDS prebuild_setup
)

# Build ${PROJECT_NAME} after executing prebuild_setup
add_executable(${PROJECT_NAME} ${source})
add_dependencies(${PROJECT_NAME} run_prebuild_setup)
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}/renderer")
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}/core")
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/resources/include/")
target_include_directories(${PROJECT_NAME} PUBLIC "${SRC_DIR}/vendor")
target_include_directories(${PROJECT_NAME} PUBLIC "${SRC_DIR}/vendor/spdlog/include")

# --- END PROJECT ---

# ==================

# --- START DEPENDENCIES ---

set(DEPS_DIR "${PROJECT_SOURCE_DIR}/dependencies")

target_include_directories(${PROJECT_NAME} PUBLIC "${DEPS_DIR}/glad/include/")

add_subdirectory("${DEPS_DIR}/glfw")
target_link_libraries(${PROJECT_NAME} "glfw")

add_subdirectory("${DEPS_DIR}/range-v3")
target_link_libraries(${PROJECT_NAME} "range-v3")

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

find_package(OpenCL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)

add_library(gladlib STATIC "${DEPS_DIR}/glad/src/gl.c")
target_include_directories(gladlib PUBLIC "${DEPS_DIR}/glad/include/")
target_link_libraries(${PROJECT_NAME} gladlib)

file(GLOB imguisource CONFIGURE_DEPENDS "${SRC_DIR}/vendor/imgui/*.cpp")
add_library(imguilib STATIC "${imguisource}")
target_link_libraries(imguilib "glfw")
target_link_libraries(${PROJECT_NAME} imguilib)

# --- END DEPENDENCIES ---
