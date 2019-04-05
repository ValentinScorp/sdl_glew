#version 330

in vec3 normal;
in vec2 uv0;
in vec2 uv1;

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
   color = texture(texture1, uv0);
}