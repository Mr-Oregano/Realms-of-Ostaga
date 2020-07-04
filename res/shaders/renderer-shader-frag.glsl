#version 460 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[32];

in vec4 vf_Color;
in vec2 vf_TextureCoords;
in vec2 vf_TexelSize;

flat in int vf_TexID;

void main()
{

	vec2 Texel_TexCoords = vf_TexelSize * vf_TextureCoords;

	vec2 alpha = 0.7 * vec2(dFdx(Texel_TexCoords.x), dFdy(Texel_TexCoords.y));
	vec2 CurrentFragInTexel = fract(Texel_TexCoords);
	vec2 CurrentTexel = floor(Texel_TexCoords);
	vec2 FilterOffset = clamp(CurrentFragInTexel / (2.0 * alpha), 0.0, 0.5)
					  + clamp((CurrentFragInTexel - 1.0) / (2.0 * alpha) + 0.5, 0.0, 0.5);

	vec2 FilteredCoords = (CurrentTexel + FilterOffset) / vf_TexelSize;

	// Texture Unit selection requires branching since behaviour is not
	// defined for non-constant indices in sampler2D arrays.
	//
	vec4 texColor = vec4(1.0);
	switch (vf_TexID)
	{
	case 0:	 texColor *= texture(u_Textures[0 ], FilteredCoords); break;
	case 1:	 texColor *= texture(u_Textures[1 ], FilteredCoords); break;
	case 2:	 texColor *= texture(u_Textures[2 ], FilteredCoords); break;
	case 3:	 texColor *= texture(u_Textures[3 ], FilteredCoords); break;
	case 4:	 texColor *= texture(u_Textures[4 ], FilteredCoords); break;
	case 5:	 texColor *= texture(u_Textures[5 ], FilteredCoords); break;
	case 6:	 texColor *= texture(u_Textures[6 ], FilteredCoords); break;
	case 7:	 texColor *= texture(u_Textures[7 ], FilteredCoords); break;
	case 8:	 texColor *= texture(u_Textures[8 ], FilteredCoords); break;
	case 9:	 texColor *= texture(u_Textures[9 ], FilteredCoords); break;
	case 10: texColor *= texture(u_Textures[10], FilteredCoords); break;
	case 11: texColor *= texture(u_Textures[11], FilteredCoords); break;
	case 12: texColor *= texture(u_Textures[12], FilteredCoords); break;
	case 13: texColor *= texture(u_Textures[13], FilteredCoords); break;
	case 14: texColor *= texture(u_Textures[14], FilteredCoords); break;
	case 15: texColor *= texture(u_Textures[15], FilteredCoords); break;
	case 16: texColor *= texture(u_Textures[16], FilteredCoords); break;
	case 17: texColor *= texture(u_Textures[17], FilteredCoords); break;
	case 18: texColor *= texture(u_Textures[18], FilteredCoords); break;
	case 19: texColor *= texture(u_Textures[19], FilteredCoords); break;
	case 20: texColor *= texture(u_Textures[20], FilteredCoords); break;
	case 21: texColor *= texture(u_Textures[21], FilteredCoords); break;
	case 22: texColor *= texture(u_Textures[22], FilteredCoords); break;
	case 23: texColor *= texture(u_Textures[23], FilteredCoords); break;
	case 24: texColor *= texture(u_Textures[24], FilteredCoords); break;
	case 25: texColor *= texture(u_Textures[25], FilteredCoords); break;
	case 26: texColor *= texture(u_Textures[26], FilteredCoords); break;
	case 27: texColor *= texture(u_Textures[27], FilteredCoords); break;
	case 28: texColor *= texture(u_Textures[28], FilteredCoords); break;
	case 29: texColor *= texture(u_Textures[29], FilteredCoords); break;
	case 30: texColor *= texture(u_Textures[30], FilteredCoords); break;
	case 31: texColor *= texture(u_Textures[31], FilteredCoords); break;
	}

	o_Color = texColor * vf_Color;

}
