#version 330

in vec3 normal;
in vec2 uv;

uniform sampler2D colorTexture;    

out vec4 color;    

void main()
{
   color = texture(colorTexture, uv);
}