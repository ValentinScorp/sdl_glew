#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUv0;
layout(location = 3) in vec2 iUv1;

out vec3 vertexNormal;
out vec2 uvTerrain;
out vec2 uvAlpha;

layout(std140) uniform cameraMatrices
{
   mat4 projection;
   mat4 view;   
};

uniform mat4 modelMatrix;

void main()
{
   gl_Position = projection * view * modelMatrix * vec4(iPosition, 1.0);
   vertexNormal = iNormal;
   uvTerrain = iUv0;
   uvAlpha = iUv1;
}