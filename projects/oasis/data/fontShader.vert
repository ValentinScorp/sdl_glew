#version 330

layout(location = 0) in vec2 iPosition;
layout(location = 1) in vec2 iTextureCoords;

out vec2 textureCoords;

uniform mat4 projectionMatrix;

void main()
{
   gl_Position = projectionMatrix * vec4(iPosition, 0.0, 1.0);
   textureCoords = iTextureCoords;   
}