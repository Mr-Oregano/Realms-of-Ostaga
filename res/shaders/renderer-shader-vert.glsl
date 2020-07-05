#version 460 core

layout(location = 0) in vec4 a_Dimensions;
layout(location = 1) in float a_Rotation;
layout(location = 2) in vec4 a_Tint;
layout(location = 3) in vec2 a_TexelSize;
layout(location = 4) in vec4 a_TexCoords;

out vec2 v_Pos;
out vec2 v_Size;
out float v_Rot;

out vec4 v_Tint;
out vec2 v_TexelSize;
out vec4 v_TexCoords;

void main()
{
	v_Pos = a_Dimensions.xy;
	v_Size = a_Dimensions.zw;
	v_Rot = a_Rotation;

	v_Tint = a_Tint;
	v_TexelSize = a_TexelSize;
	v_TexCoords = a_TexCoords;
}