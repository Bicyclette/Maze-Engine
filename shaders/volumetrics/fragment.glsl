#version 460 core

layout (location = 0) out vec4 color;

struct Light
{
	int type; // 0 => point, 1 => directional, 2=> spot
	vec3 position;
	vec3 direction;
	vec3 color;
    float kc;
    float kl;
    float kq;
	mat4 lightSpaceMatrix;
    int isVolumetric;
    int hasFog;
    float tau;
    float phi;
    float fog_gain;
};

struct Camera
{
	vec3 viewPos;
	float near_plane;
	float far_plane;
	mat4 inv_viewProj;
	sampler2D depthMap;
};

uniform float time;
uniform int N; // raymarching steps
uniform sampler2D worldPosMap;
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

const float PI = 3.14159f;

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

float getFragVisibility(vec3 worldPos, int id)
{
    Light l = light[id];
    vec4 lightPos = l.lightSpaceMatrix * vec4(worldPos, 1.0f);
    //if(l.type != 0)
        return 1.0f - fetchShadow(lightPos, l.direction, id);
    //else
        //return 1.0f - fetchOmniShadow(lightPos.xyz, l.position, id);
}

float dither_pattern[16] = float[16] (
	0.0f, 0.5f, 0.125f, 0.625f,
	0.75f, 0.22f, 0.875f, 0.375f,
	0.1875f, 0.6875f, 0.0625f, 0.5625,
	0.9375f, 0.4375f, 0.8125f, 0.3125
);

float tri( float x )
{ 
  return abs( fract(x) - .5 );
}

vec3 tri3( vec3 p )
{ 
  return vec3( 
      tri( p.z + tri( p.y * 1. ) ), 
      tri( p.z + tri( p.x * 1. ) ), 
      tri( p.y + tri( p.x * 1. ) )
  );

}

// Taken from https://www.shadertoy.com/view/4ts3z2
// By NIMITZ  (twitter: @stormoid)
float triNoise3d(in vec3 p, in float spd, in float time)
{
    float z=1.4;
	float rz = 0.;
    vec3 bp = p;
	for (float i=0.; i<=3.; i++ )
	{
        vec3 dg = tri3(bp*2.);
        p += (dg+time*spd);

        bp *= 1.8;
		z *= 1.5;
		p *= 1.2;
        //p.xz*= m2;
        
        rz+= (tri(p.z+tri(p.x+tri(p.y))))/z;
        bp += 0.14;
	}
	return rz;
}

float sample_fog(vec3 pos, int light_index)
{
	return triNoise3d(pos * 2.2 / 8, 0.075, time) * light[light_index].fog_gain;
}

void main()
{
    float fragDepth = texture(cam.depthMap, fs_in.texCoords).r;
	vec4 fragWorldPos = texture(worldPosMap, fs_in.texCoords);

	// (fragment -> camera) world space vector
	vec3 rayFrag2Cam_world = cam.viewPos - fragWorldPos.xyz;
    float rayFrag2Cam_world_size = length(rayFrag2Cam_world);

	// ray marching step
	float step = rayFrag2Cam_world_size / N;
    // ray marching direction
    vec3 raymarching_dir = normalize(rayFrag2Cam_world);

    vec3 accumFog = vec3(0.0f);
    float light_contribution = 0.0f;
    float ratio = 0.0f;
    for(int i = 0; i < lightCount; ++i)
    {
        if(light[i].isVolumetric == 0)
            continue;
        for(int j = 0; j < N; ++j)
        {
            float dither_value = dither_pattern[ (int(gl_FragCoord.x) % 4)* 4 + (int(gl_FragCoord.y) % 4) ];
            vec3 current_position_world = fragWorldPos.xyz + dither_value * (raymarching_dir * step * j);
            float visibility = getFragVisibility(current_position_world, i);
            ratio += visibility;
            float d = length(current_position_world - light[i].position);
            float d_rcp = 1.0f/d;
            float fog = (light[i].hasFog == 1) ? sample_fog(current_position_world, i) : 1.0f;
            if(light[i].type != 0)
                light_contribution += fog * light[i].tau * (visibility * (light[i].phi * 0.25 * (1.0f/PI)) * d_rcp) * step;
            else
            {
                float intensity = 1.0 / (light[i].kc + light[i].kl * d + light[i].kq * (d*d));
                float ambient_fog_intensity = mix(1.0, 0.0, min(length(cam.viewPos - current_position_world), 10.0)/10.0);
                float ambient_fog = 0.15 * fog * ambient_fog_intensity;
                light_contribution += fog * light[i].tau * intensity * (visibility * (light[i].phi * 0.25 * (1.0f/PI)) * d_rcp * d_rcp + ambient_fog) * step;
            }
        }
        accumFog += min(1.0, light_contribution) * light[i].color;
        accumFog = (light[i].type == 0) ? accumFog * ratio/N : accumFog;
        light_contribution = 0.0f;
        ratio = 0.0f;
    }
	color = vec4(accumFog, min(1.0f, fragDepth));
}
