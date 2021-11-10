#shader vertex
#version 330 core
in vec3 in_Position;
uniform mat4 MVP;
void main()
{
   gl_Position = MVP * vec4(in_Position, 1.0);
}

#shader fragment
#version 330 core
out vec4 color;
uniform vec4 u_Color;
void main()
{
   color = u_Color;
}
