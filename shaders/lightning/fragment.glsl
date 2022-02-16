#version 460 core

out vec4 fragColor;

uniform vec3 color;
uniform float intensity;

void main()
{
	fragColor = vec4(color * intensity, 1.0f);
}
