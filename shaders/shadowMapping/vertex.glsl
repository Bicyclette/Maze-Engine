#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
layout (location = 5) in ivec4 boneID;
layout (location = 6) in vec4 boneWeight;
layout (location = 7) in mat4 instanceModel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform bool computeWorldPos;
uniform bool instancing;

out VS_OUT
{
	vec2 texCoords;
}vs_out;

//#################### ANIMATION DATA ####################
uniform int animated;
const int MAX_BONES = 50;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bonesMatrices[MAX_BONES];
//########################################################

void main()
{
	vs_out.texCoords = aTex;
	vec4 position = vec4(aPos, 1.0f);
	if(animated == 1)
	{
		mat4 boneTransform = mat4(0.0f);
		for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			boneTransform += bonesMatrices[boneID[i]] * boneWeight[i];
		}
		position = boneTransform * vec4(aPos, 1.0f);
	}

	if(instancing)
	{
		if(computeWorldPos)
			gl_Position = instanceModel * position;
		else
			gl_Position = proj * view * instanceModel * position;
	}
	else
	{
		if(computeWorldPos)
			gl_Position = model * position;
		else
			gl_Position = proj * view * model * position;
	}

}
