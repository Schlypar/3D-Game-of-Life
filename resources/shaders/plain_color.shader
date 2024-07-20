#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
out vec4 i_color;

uniform mat4 Model;
uniform mat4 ProjectionView;
uniform vec4 Color;

void main()
{
    i_color = Color;
    gl_Position = ProjectionView * Model * position;
};

#shader fragment
#version 330 core

in vec4 i_color;
out vec4 color;

void main()
{
    color = i_color;
};
