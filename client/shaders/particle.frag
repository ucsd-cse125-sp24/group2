#version 330 core

in vec2 TexCoords;

uniform sampler2D texture0;

out vec4 fragColor;

uniform vec4 color;

uniform bool hasTexture;

void main()
{
	if(hasTexture) {
		fragColor = texture(texture0, TexCoords);
	} else {
		fragColor = color;
	}
}