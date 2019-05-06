#version 330

layout(location = 0) in vec2 iPosition;

uniform mat4 projectionMatrix;
uniform vec2 position;

void main()
{
   gl_Position = projectionMatrix * vec4(iPosition + position, -0.01, 1.0);   
}