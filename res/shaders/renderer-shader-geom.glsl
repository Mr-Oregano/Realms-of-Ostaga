#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_ViewProjection;

in vec2 v_Pos[];
in vec2 v_Size[];
in float v_Rot[];

in vec4 v_Color[];
flat in int v_TexID[];
in vec2 v_TexelSize[];

out vec4 vf_Color;
out vec2 vf_TextureCoords;
flat out int vf_TexID;
out vec2 vf_TexelSize;

void CreateVertex(vec2 center, vec2 pos, float rotation)
{
	vec2 posRot;

	posRot.x = pos.x * cos(rotation) - pos.y * sin(rotation);
	posRot.y = pos.x * sin(rotation) + pos.y * cos(rotation);

	gl_Position = u_ViewProjection * vec4(posRot + center, 0.0, 1.0);
	EmitVertex();
}
void main()
{
	// Forward to fragment shader
	vf_TexID = v_TexID[0];
	vf_TexelSize = v_TexelSize[0];
	vf_Color = v_Color[0];
	//

	vf_TextureCoords = vec2(0.0, 0.0);
	CreateVertex(v_Pos[0], v_Size[0] * vec2(-0.5,  0.5), v_Rot[0]);

	vf_TextureCoords = vec2(0.0, 1.0);
	CreateVertex(v_Pos[0], v_Size[0] * vec2(-0.5, -0.5), v_Rot[0]);

	vf_TextureCoords = vec2(1.0, 0.0);
	CreateVertex(v_Pos[0], v_Size[0] * vec2( 0.5,  0.5), v_Rot[0]);

	vf_TextureCoords = vec2(1.0, 1.0);
	CreateVertex(v_Pos[0], v_Size[0] * vec2( 0.5, -0.5), v_Rot[0]);

	EndPrimitive();
}
