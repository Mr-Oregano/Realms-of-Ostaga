#version 460 core

layout(location = 0) in vec4 a_Dimensions;
layout(location = 1) in float a_Rotation;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexID;
layout(location = 4) in vec2 a_TexelSize;

out vec2 v_Pos;
out vec2 v_Size;
out float v_Rot;

out vec4 v_Color;
flat out int v_TexID;
out vec2 v_TexelSize;

void main()
{
	v_Pos = a_Dimensions.xy;
	v_Size = a_Dimensions.zw;
	v_Rot = a_Rotation;

	v_Color = a_Color;
	v_TexID = int(a_TexID);
	v_TexelSize = a_TexelSize;
}