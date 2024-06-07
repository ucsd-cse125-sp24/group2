#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;
uniform float aspectRatio; 

out vec2 TexCoords;

void main()
{   
    vec3 adjustedPosition = position;
    adjustedPosition.x *= aspectRatio;
    adjustedPosition.y /= aspectRatio;
    if(aspectRatio > 1.34) {
        adjustedPosition.x = position.x;
        adjustedPosition.y = position.y;
    }

    // for some reason using stbi_set_flip_vertically_on_load(true) messes up the texture of animated model 
    // so I need to flip it manually here
    TexCoords = vec2(texCoords.x, 1 - texCoords.y);
    gl_Position = model * vec4(adjustedPosition.x, adjustedPosition.y, 0.0, 1.0);
}