#shader vertex
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec4 in_Color;
uniform mat4 MVP;

out vec4 out_Color;
void main()
{
   out_Color = in_Color;
   gl_Position = MVP * vec4(in_Position, 1.0);
}

#shader fragment
#version 330 core

in vec4 out_Color;
out vec4 frag_Color;
//uniform vec4 u_Color;
void main()
{
   // out_Color = vec4(0,1,0,1);
   frag_Color = out_Color;
}
