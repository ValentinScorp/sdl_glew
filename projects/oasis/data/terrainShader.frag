#version 330

in vec3 normal;
in vec2 uvTerrain;
in vec2 uvAlpha;

out vec4 color; 

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D textureAlpha;

void main()
{	
	vec4 texColor0 = texture(texture0, uvTerrain);
	vec4 texColor1 = texture(texture1, uvTerrain);
	vec4 texColor2 = texture(texture2, uvTerrain);
	vec4 texColor3 = texture(texture3, uvTerrain);
	vec4 texColor4 = texture(texture4, uvTerrain);
	
	vec2 flipVertUvAlpha = vec2(uvAlpha.s,  1.0 - uvAlpha.t);
	vec2 flipHorizUvAlpha = vec2(1.0 - uvAlpha.s, uvAlpha.t);
	vec2 flipDiagUvAlpha = vec2(1.0 - uvAlpha.s,  1.0 - uvAlpha.t);
	
	vec4 texAlpha = texture(textureAlpha, uvAlpha);
	vec4 texAlphaVert = texture(textureAlpha, flipVertUvAlpha);
	vec4 texAlphaDiag = texture(textureAlpha, flipDiagUvAlpha);
	vec4 texAlphaHoriz = texture(textureAlpha, flipHorizUvAlpha);
	
	vec4 color0 = mix(texColor1, texColor0, texAlpha.r);
	vec4 color1 = mix(texColor2, color0, texAlphaVert.r);
	vec4 color2 = mix(texColor3, color1, texAlphaDiag.r);
	
	color = mix(texColor4, color2, texAlphaHoriz.r);
}