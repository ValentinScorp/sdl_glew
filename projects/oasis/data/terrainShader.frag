#version 330

in vec3 normal;
in vec2 uvTerrain;
in vec2 uvAlpha;

out vec4 color; 

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D textureAlphaSide;
uniform sampler2D textureAlphaCorner;
uniform sampler2D textureAlphaFull;
uniform sampler2D textureAlphaDiag;

void main()
{	
	vec4 grass = texture(texture0, uvTerrain);
	vec4 sand = texture(texture1, uvTerrain);
	vec4 rock = texture(texture2, uvTerrain);
	
	vec2 flipVertUvAlpha = vec2(uvAlpha.s,  1.0 - uvAlpha.t);
	vec2 flipHorizUvAlpha = vec2(1.0 - uvAlpha.s, uvAlpha.t);
	vec2 flipDiagUvAlpha = vec2(1.0 - uvAlpha.s,  1.0 - uvAlpha.t);
	
	vec4 texel3 = texture(textureAlphaCorner, uvAlpha);
	vec4 texel4 = texture(textureAlphaCorner, flipVertUvAlpha);
	vec4 texel5 = texture(textureAlphaCorner, flipDiagUvAlpha);
	vec4 texel6 = texture(textureAlphaCorner, flipHorizUvAlpha);
	
	vec4 color0, color1, color2, color3;

	color0 = mix(sand, grass, texel3.r);
	color1 = mix(rock, color0, texel4.r);
	color2 = mix(sand, color1, texel5.r);
	color  = mix(grass, color2, texel6.r);
	
}