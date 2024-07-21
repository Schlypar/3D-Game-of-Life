// DO NOT EDIT: GENERATED FILE
#pragma once
#include <string>

const std::string __shader_test = "\
#shader vertex\n\
#version 330 core\n\
\n\
layout (location = 0) in vec4 position;\n\
layout (location = 1) in vec4 color;\n\
out vec4 i_color;\n\
\n\
uniform mat4 Model;\n\
uniform mat4 ProjectionView;\n\
\n\
void main()\n\
{\n\
    i_color = color;\n\
    gl_Position = ProjectionView * Model * position;\n\
};\n\
\n\
#shader fragment\n\
#version 330 core\n\
\n\
in vec4 i_color;\n\
out vec4 color;\n\
\n\
void main()\n\
{\n\
    color = i_color;\n\
};\n\
";