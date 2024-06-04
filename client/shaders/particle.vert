#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

out vec2 TexCoords;

void main()
{   
    // for some reason using stbi_set_flip_vertically_on_load(true) messes up the texture of animated model 
    // so I need to flip it manually here
    //TexCoords = vec2(texCoords.x, 1 - texCoords.y);
    TexCoords = texCoords;
    gl_Position = viewProj * model * vec4(position, 1.0);
}