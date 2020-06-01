#version 460 core

layout(location = 0) in vec4 a_Position;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_Model * a_Position;
}