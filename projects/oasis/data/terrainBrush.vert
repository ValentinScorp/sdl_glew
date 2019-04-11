#version 330

layout(location = 0) in vec3 position;

layout(std140) uniform cameraMatrices
{
   mat4 projection;
   mat4 view;   
};

uniform mat4 modelMatrix;

void main()
{
   gl_Position = projection * view * modelMatrix * vec4(position, 1.0);
}