#version 460 core

out vec4 axisColor;

uniform vec3 color;

void main()
{
	axisColor = vec4(color, 1.0f);
}
