#version 330

layout(location = 0) in vec2 iPosition;

uniform mat4 projectionMatrix;

void main()
{
   gl_Position = projectionMatrix * vec4(iPosition, -0.01, 1.0);   
}