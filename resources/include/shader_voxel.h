// DO NOT EDIT: GENERATED FILE
#pragma once
#include <string>

namespace GoL {

const std::string SHADER_VOXEL = "\
#shader vertex\n\
#version 330 core\n\
layout (location = 0) in vec3 vertex;\n\
layout (location = 1) in vec3 color;\n\
layout (location = 2) in mat4 Matrix;\n\
uniform mat4 ProjectionView;\n\
out vec4 i_color;\n\
void main()\n\
{\n\
  i_color = vec4(color.x, color.y, color.z, 0.0);\n\
  gl_Position = ProjectionView*Matrix*vec4(vertex.x, vertex.y, vertex.z, 1.0);\n\
}\n\
\n\
#shader fragment\n\
#version 330 core\n\
in vec4 i_color;\n\
out vec4 FragColor;\n\
void main()\n\
{\n\
  FragColor = i_color;\n\
}\n\
";

}