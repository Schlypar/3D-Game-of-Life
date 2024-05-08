#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in mat4 instanceMatrix;
out vec4 i_color;

uniform mat4 ProjectionView;

void main()
{
    i_color = color;
    gl_Position = ProjectionView * instanceMatrix * position;
};

#shader fragment
#version 330 core

in vec4 i_color;
out vec4 color;

void main()
{
    color = i_color;
};