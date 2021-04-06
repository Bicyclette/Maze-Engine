#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 instanceModel;

out VS_OUT
{
	vec2 texCoords;
	vec3 normal;
	vec3 fragPos;
	mat4 viewMatrix;
	mat4 projMatrix;
} vs_out;

uniform mat4 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform bool instancing;

void main()
{
	vs_out.texCoords = aTex;
	if(instancing)
	{
		gl_Position = proj * view * instanceModel * vec4(aPos, 1.0f);
		vs_out.normal = mat3(transpose(inverse(instanceModel))) * aNorm;
		vs_out.fragPos = vec3(instanceModel * vec4(aPos, 1.0f));
	}
	else
	{
		gl_Position = proj * view * model * vec4(aPos, 1.0f);
		vs_out.normal = mat3(normalMatrix) * aNorm;
		vs_out.fragPos = vec3(model * vec4(aPos, 1.0f));
	}
	vs_out.viewMatrix = view;
	vs_out.projMatrix = proj;
}
