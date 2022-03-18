#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out VS_OUT
{
	vec2 texCoords;
} vs_out;

uniform mat4 model;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	vs_out.texCoords = aTex;
}
