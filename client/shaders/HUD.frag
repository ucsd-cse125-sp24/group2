#version 330 core

in vec2 TexCoords;

uniform sampler2D texture0;
uniform sampler2D texture1; 
uniform float ratio;
out vec4 fragColor;
uniform vec3 color;
uniform bool isHealthBar;

void main()
{
	if(isHealthBar) {
		if(TexCoords.x > ratio)
		{
			fragColor = texture(texture0, TexCoords);
		} else {
			fragColor = texture(texture1, TexCoords);
		}
	} else {
		fragColor = texture(texture0, TexCoords);
	}
}