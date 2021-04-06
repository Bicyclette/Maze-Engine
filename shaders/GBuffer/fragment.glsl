#version 460 core

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragNormal;
layout (location = 2) out vec4 fragAlbedo;

in VS_OUT
{
	vec3 normal;
	vec3 fragPos;
} fs_in;

void main()
{
	fragPos = fs_in.fragPos;
	fragNormal = normalize(fs_in.normal);
	fragAlbedo = vec4(vec3(0.95f), 1.0f);
}
