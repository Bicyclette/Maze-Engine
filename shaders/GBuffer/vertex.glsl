#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 boneID;
layout (location = 6) in vec4 boneWeight;
layout (location = 7) in mat4 instanceModel;

out VS_OUT
{
	vec3 normal;
	vec3 fragPosView;
    vec3 fragPosWorld;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform bool instancing;

//#################### ANIMATION DATA ####################
uniform int animated;
const int MAX_BONES = 50;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bonesMatrices[MAX_BONES];
//########################################################

void main()
{
	vec4 position = vec4(aPos, 1.0f);
	vec4 normal = vec4(aNorm, 0.0f);
	if(animated == 1)
	{
		mat4 boneTransform = mat4(0.0f);
		for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			boneTransform += bonesMatrices[boneID[i]] * boneWeight[i];
		}
		position = boneTransform * vec4(aPos, 1.0f);
		normal = boneTransform * vec4(aNorm, 0.0f);
	}

	if(instancing)
	{
		gl_Position = proj * view * instanceModel * position;
		vs_out.normal = vec3(transpose(inverse(view * instanceModel)) * normal);
		vs_out.fragPosView = vec3(view * instanceModel * position);
		vs_out.fragPosWorld = vec3(instanceModel * position);
	}
	else
	{
		gl_Position = proj * view * model * position;
		vs_out.normal = vec3(transpose(inverse(view * model)) * normal);
		vs_out.fragPosView = vec3(view * model * position);
		vs_out.fragPosWorld = vec3(model * position);
	}
}
