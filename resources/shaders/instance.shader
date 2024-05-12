#shader vertex
#version 330 core
layout (location = 0) in vec3 Vertex;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 Matrix;
uniform mat4 ProjectionView;
out vec4 i_color;
void main()
{
  i_color = vec4(color.x, color.y, color.z, 0.0);
  gl_Position = ProjectionView*Matrix*vec4(Vertex.x, Vertex.y, Vertex.z, 1.0);
}

#shader fragment
#version 330 core
in vec4 i_color;
out vec4 FragColor;
void main()
{
  FragColor = i_color;
}