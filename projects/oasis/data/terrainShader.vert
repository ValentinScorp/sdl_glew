#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUv0;
layout(location = 3) in vec2 iUv1;

out vec3 normal;
out vec2 uv0;
out vec2 uv1;

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
   uv0 = iUv0;
   uv1 = iUv1;
}