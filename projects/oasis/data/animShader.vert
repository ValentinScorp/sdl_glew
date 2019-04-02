#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUv;

out vec3 normal;
out vec2 uv;

layout(std140) uniform GlobalMatrices
{
   mat4 cameraToClipMatrix;
   mat4 worldToCameraMatrix;
};

uniform mat4 modelToWorldMatrix;

void main()
{
   gl_Position = cameraToClipMatrix * worldToCameraMatrix * modelToWorldMatrix * vec4(iPosition, 1.0);
   normal = iNormal;
   uv = iUv;
}