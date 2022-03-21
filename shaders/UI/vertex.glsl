#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoords;

uniform mat4 proj;

void main()
{
	gl_Position = proj * vec4(aPos, 1.0f);
    texCoords = aTex;
}
