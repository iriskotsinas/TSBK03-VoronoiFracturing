#shader vertex
#version 330 core

layout (location = 0) in vec3 in_Position;
uniform mat4 MVP;

out vec4 out_Color;
void main()
{
   gl_Position = MVP * vec4(in_Position, 1.0);
}

#shader fragment
#version 330 core

out vec4 frag_Color;
uniform vec4 u_Color;
void main()
{
   frag_Color = u_Color;
}
