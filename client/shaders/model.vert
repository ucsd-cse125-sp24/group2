#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. 
out vec3 fragNormal;
out vec2 TexCoords;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIDs[i] == -1) 
            continue;
        if(boneIDs[i] >=MAX_BONES) 
        {
            totalPosition = vec4(position,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIDs[i]] * vec4(position,1.0f);
        totalPosition += localPosition * weights[i];
        //vec3 localNormal = mat3(finalBonesMatrices[boneIDs[i]]) * norm;
   }
    TexCoords = texCoords;

    gl_Position = viewProj * model * vec4(position, 1.0);
    
	fragNormal = vec3(model * vec4(normal, 0));
    
}