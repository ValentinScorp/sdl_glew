#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUv;

out vec3 normal;
out vec2 uv;

layout(std140) uniform cameraMatrices
{
   mat4 projection;
   mat4 view;   
};

uniform mat4 modelMatrix;

void main()
{
   gl_Position = projection * view * modelMatrix * vec4(iPosition, 1.0);
   normal = iNormal;
   uv = iUv;
}