#version 330 core

in vec2 TexCoords;

//uniform sampler2D texture0;
//uniform sampler2D texture1; 
uniform sampler2D textures[4];
uniform int textureIndex;
uniform float ratio;
out vec4 fragColor;
uniform vec3 color;
uniform bool isHealthBar;
uniform float opacity;

void main()
{
	if(isHealthBar) {
		if(TexCoords.x > ratio)
		{
			fragColor = texture(textures[0], TexCoords);
		} else {
			fragColor = texture(textures[1], TexCoords);
		}
	} else {
		fragColor = vec4(1.0f, 1.0f, 1.0f, opacity) * texture(textures[textureIndex], TexCoords);
	}
}