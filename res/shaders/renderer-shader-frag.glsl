#version 460 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Atlas;

in vec4 vf_Tint;
in vec2 vf_TexCoords;
in vec2 vf_TexelSize;

void main()
{
	vec2 Texel_TexCoords = vf_TexelSize * vf_TexCoords;

	vec2 alpha = 0.7 * vec2(dFdx(Texel_TexCoords.x), dFdy(Texel_TexCoords.y));
	vec2 CurrentFragInTexel = fract(Texel_TexCoords);
	vec2 CurrentTexel = floor(Texel_TexCoords);
	vec2 FilterOffset = clamp(CurrentFragInTexel / (2.0 * alpha), 0.0, 0.5)
					  + clamp((CurrentFragInTexel - 1.0) / (2.0 * alpha) + 0.5, 0.0, 0.5);

	vec2 FilteredCoords = (CurrentTexel + FilterOffset) / vf_TexelSize;

	o_Color = texture(u_Atlas, FilteredCoords) * vf_Tint;
}
