#version 460 core

layout (location = 0) out vec4 fragPosView;
layout (location = 1) out vec4 fragNormal;
layout (location = 2) out vec4 fragDepth;
layout (location = 3) out vec4 fragPosWorld;

in VS_OUT
{
	vec3 normal;
	vec3 fragPosView;
	vec3 fragPosWorld;
} fs_in;

void main()
{
	fragPosView = vec4(fs_in.fragPosView, 1.0f);
	fragNormal = vec4(normalize(fs_in.normal), 1.0f);
	fragDepth = vec4(vec3(gl_FragCoord.z), 1.0f);
	fragPosWorld = vec4(fs_in.fragPosWorld, 1.0f);
}
