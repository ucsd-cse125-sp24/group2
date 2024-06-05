#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

// Uniform variables
uniform mat4 viewMtx;
uniform mat4 projMtx;
uniform mat4 model;

out vec2 TexCoords;

void main()
{   
    TexCoords = texCoords;
    gl_Position = projMtx * viewMtx * model * vec4(position, 1.0);
}