#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUv0;
layout(location = 3) in vec2 iUv1;
layout(location = 4) in vec4 iTerrainIndexes;

out vec3 normal;
out vec2 uvTerrain;
out vec2 uvAlpha;
flat out int terrainId0;
flat out int terrainId1;
flat out int terrainId2;
flat out int terrainId3;

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
   uvTerrain = iUv0;
   uvAlpha = iUv1;
   terrainId0 = int(iTerrainIndexes.x);
   terrainId1 = int(iTerrainIndexes.y);
   terrainId2 = int(iTerrainIndexes.z);
   terrainId3 = int(iTerrainIndexes.w);
}