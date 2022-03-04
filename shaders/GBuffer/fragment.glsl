#version 460 core

layout (location = 0) out vec4 fragPos;
layout (location = 1) out vec4 fragNormal;
layout (location = 2) out vec4 fragAlbedo;

in VS_OUT
{
	vec3 normal;
	vec3 fragPos;
} fs_in;

void main()
{
	fragPos = vec4(fs_in.fragPos, 1.0f);
	fragNormal = vec4(normalize(fs_in.normal), 1.0f);
	fragAlbedo = vec4(vec3(0.95f), 1.0f);
}
