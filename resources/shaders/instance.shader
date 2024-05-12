#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 instanceMatrix;
out vec4 i_color;

uniform mat4 ProjectionView;

void main()
{
    i_color = vec4(color.x, color.y, color.z, 0.0);
    gl_Position = ProjectionView * instanceMatrix * vec4(position.x, position.y, position.z, 1.0);
};

#shader fragment
#version 330 core

in vec4 i_color;
out vec4 color;

void main()
{
    color = i_color;
};