#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
	vec4 up;
	vec4 right;
	vec4 front;
} vs_out;

void main()
{
	vs_out.up = proj * view * model * vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vs_out.right = proj * view * model * vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vs_out.front = proj * view * model * vec4(0.0f, 0.0f, 1.0f, 0.0f);
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
}
