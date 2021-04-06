#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 direction;
uniform vec3 right;
uniform float cutOff;

out VS_OUT
{
	vec4 direction;
	vec4 right;
	vec4 up;
	float cutOff;
} vs_out;

void main()
{
	vs_out.direction = proj * view * model * vec4(direction, 0.0f);
	vs_out.right = proj * view * model * vec4(right, 0.0f);
	vs_out.up = proj * view * model * vec4(cross(right, direction), 0.0f);
	vs_out.cutOff = cutOff;
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
}
