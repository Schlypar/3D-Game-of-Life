cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

project(test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# --- START PROJECT ---

file(GLOB source CONFIGURE_DEPENDS "${SRC_DIR}/*")

add_executable(${PROJECT_NAME} ${source})

# --- END PROJECT ---

# ==================

# --- START DEPENDENCIES ---

set(DEPS_DIR "${PROJECT_SOURCE_DIR}/dependencies")

add_subdirectory("${DEPS_DIR}/glfw")
target_link_libraries(${PROJECT_NAME} "glfw")

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

add_library(gladlib STATIC "${DEPS_DIR}/glad/src/gl.c")
target_include_directories(gladlib PUBLIC "${DEPS_DIR}/glad/include/")

target_link_libraries(${PROJECT_NAME} gladlib)
target_include_directories(${PROJECT_NAME} PRIVATE "${DEPS_DIR}/glad/include/")

# --- END DEPENDENCIES ---
