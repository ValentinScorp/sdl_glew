#version 330

in vec2 textureCoords;
out vec4 color; 

uniform sampler2D texture0;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture0, textureCoords).r);
	color = vec4(textColor, 1.0) * sampled;
}