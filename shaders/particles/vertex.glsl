#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 velocity;
layout (location = 2) in float lifetime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 camRight;
uniform vec3 camUp;

out VS_OUT
{
	vec4 up;
	vec4 right;
	float lifetime;
} vs_out;

void main()
{
	vs_out.up = proj * view * model * vec4(camRight, 0.0f);
	vs_out.right = proj * view * model * vec4(camUp, 0.0f);
	vs_out.lifetime = lifetime;
	gl_Position = proj * view * model * vec4(pos, 1.0f);
}
