#version 460 core

out vec4 color;

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
	mat4 inv_view;
	mat4 inv_proj;
};

uniform	sampler2D fragPosition; // world
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

const float PI = 3.14159265359;
const float G_SCATTERING = 0.5f;

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

// Henyey-greenstein phase function
float computeScattering(float lightDotView)
{
	float result = 1.0f - G_SCATTERING * G_SCATTERING;
	result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * lightDotView, 1.5f));
	return result;
}

vec3 depthToWorldPosition(sampler2D shadowMap)
{
	// get fragment depth range = [0 - 1]
	float fragDepth = texture(shadowMap, fs_in.texCoords).r;
	// clip space coordinates
	vec2 clipXY = fs_in.texCoords * 2.0 - 1.0;
	vec4 clip_space_coords = vec4(clipXY, fragDepth * 2.0 - 1.0, 1.0);
	// view space coordinates
	vec4 view_space_coords = cam.inv_proj * clip_space_coords;
	view_space_coords /= view_space_coords.w;
	// world space coordinates
	vec4 fragWorldPos = cam.inv_view * view_space_coords;

	return fragWorldPos.xyz;
}

void main()
{
	vec3 worldCoords = texture(fragPosition, fs_in.texCoords).xyz;
	vec4 fragWorldPos = vec4(worldCoords, 1.0f);

	// (camera -> fragment) world vector
	vec3 rayWorld = fragWorldPos.xyz - cam.viewPos;

	// ray marching with 10 steps
	vec3 step = rayWorld / 10.0f;
	
	for(int i = 0; i < lightCount; ++i)
	{
		if(light[i].type == 0) // omni
		{
			vec3 lightDirection = fragWorldPos.xyz - light[i].position;
			for(int j = 0; j <= 10; ++j)
			{
				vec3 currentWorldSpacePosition = cam.viewPos + step * j;
				// sample cube shadow map in light view space
				vec4 currentLightSpacePosition = light[i].lightSpaceMatrix * vec4(currentWorldSpacePosition, 1.0f);
				float shadow = fetchOmniShadow(currentLightSpacePosition.xyz, light[i].position, i);
				if(shadow == 0.0f)
				{
					color += vec4(computeScattering(dot(rayWorld, lightDirection)) * light[i].color, 0.0f);
				}
			}
		}
		else // not omni
		{
			for(int j = 0; j <= 10; ++j)
			{
				vec3 currentWorldSpacePosition = cam.viewPos + step * j;
				// sample shadow map in light view space
				vec4 currentLightSpacePosition = light[i].lightSpaceMatrix * vec4(currentWorldSpacePosition, 1.0f);
				currentLightSpacePosition /= currentLightSpacePosition.w;
				float shadow = fetchShadow(currentLightSpacePosition, light[i].direction, i);
				if(shadow == 0.0f)
				{
					color += vec4(computeScattering(dot(rayWorld, light[i].direction)) * light[i].color, 0.0f);
				}
			}
		}
	}
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
