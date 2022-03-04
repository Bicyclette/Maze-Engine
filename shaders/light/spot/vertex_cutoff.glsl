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
	mat4 MVP = proj * view * model;
	vs_out.direction = MVP * vec4(direction, 0.0f);
	vs_out.right = MVP * vec4(right, 0.0f);
	vs_out.up = MVP * vec4(cross(right, direction), 0.0f);
	vs_out.cutOff = cutOff;
	gl_Position = MVP * vec4(aPos, 1.0f);
}
