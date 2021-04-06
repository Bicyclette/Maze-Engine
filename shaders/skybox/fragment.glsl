#version 460 core

out vec4 color;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{
	color = texture(skybox, texCoords);
}
