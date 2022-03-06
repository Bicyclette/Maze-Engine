#version 460 core

layout (location = 0) out vec4 color;

struct Light
{
	int type; // 0 => point, 1 => directional, 2=> spot
	vec3 position;
	vec3 direction;
	vec3 color;
	mat4 lightSpaceMatrix;
};

struct Camera
{
	vec3 viewPos;
	float near_plane;
	float far_plane;
	mat4 inv_viewProj;
	sampler2D depthMap;
};

uniform int N; // raymarching steps
uniform Camera cam;
uniform int lightCount;
uniform Light light[10];
uniform sampler2D depthMap[10];
uniform samplerCube omniDepthMap[10];
uniform int pointLightCount;

in VS_OUT
{
	vec2 texCoords;
} fs_in;

float fetchShadow(vec4 fragPosLightSpace, vec3 lightDir, int l)
{
	float shadow = 0.0;
	float bias = 0.005;

	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
	// transform to [0,1] range
	projCoords = (projCoords * 0.5) + 0.5;
	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
		return shadow;
	}

	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	float depth = texture(depthMap[l - pointLightCount], projCoords.xy).r;
	shadow = (currentDepth - bias) > depth ? 1.0 : 0.0;
	return shadow;
}

float fetchOmniShadow(vec3 fragPos, vec3 lightPos, int l)
{
	float shadow = 0.0;
	float bias = 0.1;

	vec3 distFragLight = fragPos - lightPos;
	float currentDepth = length(distFragLight);
	
	float closestDepth = texture(omniDepthMap[l], distFragLight).r;
	closestDepth *= cam.far_plane; // times far_plane because closestDepth sits in range [0 - 1]
	if((currentDepth - bias) > closestDepth)
		shadow = 1.0;

	return shadow;
}

float linearizeDepth(float depth)
{
	float z_n = 2.0 * depth -1.0;
	return 2.0 * cam.near_plane * cam.far_plane / (cam.far_plane + cam.near_plane - z_n * (cam.far_plane - cam.near_plane));
}

vec4 depthToWorldPosition(sampler2D shadowMap)
{
	// get fragment depth range = [0 - 1]
	float fragDepth = texture(shadowMap, fs_in.texCoords).r;
	// clip space coordinates
	vec2 uv = fs_in.texCoords;
	vec4 clip;
	clip.x = 2.0 * uv.x - 1.0;
	clip.y = 2.0 * uv.y - 1.0;
	clip.z = 2.0 * fragDepth - 1.0;
	clip.w = 1.0;
	// world space coordinates
	vec4 fragWorldPos = cam.inv_viewProj * clip;
	return fragWorldPos /= fragWorldPos.w;
}

void main()
{
	vec4 fragWorldPos = depthToWorldPosition(cam.depthMap);

	// (camera -> fragment) world vector
	vec3 rayWorld = fragWorldPos.xyz - cam.viewPos;

	// ray marching step vector
	vec3 step = rayWorld / N;

	float fog = 0.0f;
	for(int i = 0; i < lightCount; ++i)
	{
		if(light[i].type == 0) // omni
		{
			for(int j = 0; j <= N; ++j)
			{
				vec3 currentWorldSpacePosition = cam.viewPos + step * j;
				// sample cube shadow map in light view space
				vec4 currentLightSpacePosition = light[i].lightSpaceMatrix * vec4(currentWorldSpacePosition, 1.0f);
				float shadow = fetchOmniShadow(currentLightSpacePosition.xyz, light[i].position, i);
				if(shadow == 0.0f)
				{
					//fog += 1.0/N;
				}
			}
		}
		if(light[i].type != 0)// not omni
		{
			for(int j = 0; j <= N; ++j)
			{
				vec3 currentWorldSpacePosition = cam.viewPos + step * j;
				// sample shadow map in light view space
				vec4 currentLightSpacePosition = light[i].lightSpaceMatrix * vec4(currentWorldSpacePosition, 1.0f);
				currentLightSpacePosition /= currentLightSpacePosition.w;
				float shadow = fetchShadow(currentLightSpacePosition, light[i].direction, i);
				if(shadow == 0.0f)
				{
					fog += 1.0/N;
				}
			}
		}
	}
	color = vec4(vec3(fog), 1.0f);
}
