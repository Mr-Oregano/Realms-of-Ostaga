#version 460 core

layout(location = 0) in vec4 a_Position;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * u_Model * a_Position;
}