#version 460 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

struct Light
{
	int type; // 0 => point, 1 => directional, 2 => spot
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float kc;
	float kl;
	float kq;
	vec3 color;
	mat4 lightSpaceMatrix;
};

struct Material
{
	vec3 albedo;
	float metallic;
	float roughness;
	float opacity;
	sampler2D albedoMap;
	int hasAlbedo;
	sampler2D metallicRoughMap;
	int hasMetallicRough;
	sampler2D normalMap;
	int hasNormal;
	vec3 emissiveColor;
	float emissionIntensity;
	sampler2D emissionMap;
	int hasEmission;
	int nbTextures;
};

struct Camera
{
	vec3 viewPos;
};

in VS_OUT
{
	vec2 texCoords;
	vec3 normal;
	vec3 fragPos;
	mat4 viewMatrix;
	mat4 projMatrix;
} fs_in;

uniform Camera cam;

uniform int shadowOn;
uniform int lightCount;
uniform Light light[10];
uniform sampler2D depthMap[10];
uniform samplerCube omniDepthMap[10];
uniform int pointLightCount;

uniform Material material;
uniform sampler2D ssao;
uniform int hasSSAO;
uniform vec2 viewport;

uniform int IBL;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

const float PI = 3.14159265359;
mat3 TBN;
// ----------------------------------------------------------------------------
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normalMap, fs_in.texCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.fragPos);
    vec3 Q2  = dFdy(fs_in.fragPos);
    vec2 st1 = dFdx(fs_in.texCoords);
    vec2 st2 = dFdy(fs_in.texCoords);

    vec3 N   = normalize(fs_in.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
float calculateShadow(vec4 fragPosLightSpace, vec3 lightDir, int l)
{
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthMap[l - pointLightCount], 0);
	float bias = max(0.005 * (1.0 - dot(fs_in.normal, -lightDir)), 0.0005);

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
	
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float depth = texture(depthMap[l - pointLightCount], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth - bias) > depth ? 1.0 : 0.0;
		}
	}
	
	return shadow / 9.0;
}
// ----------------------------------------------------------------------------
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float calculateOmniShadow(vec3 fragPos, vec3 lightPos, int l)
{
	float shadow = 0.0;
	float bias = 0.1;
	int samples = 20;
	float viewDistance = length(cam.viewPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / 100.0f)) / 25.0;
	
	vec3 distFragLight = fragPos - lightPos;
	float currentDepth = length(distFragLight);
	
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(omniDepthMap[l], distFragLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= 100.0f;
		if(currentDepth - bias > closestDepth)
			shadow += 1.0f;
	}

	shadow /= float(samples);

	return shadow;
}
// ----------------------------------------------------------------------------
void main()
{
	// early discard
	if(material.opacity == 0.0f)
		discard;
	if(material.nbTextures > 0)
	{
		if(texture(material.albedoMap, fs_in.texCoords).a == 0.0f)
			discard;
	}

	vec3 albedo;
	float metallic;
	float roughness;
	vec2 fragCoords = gl_FragCoord.xy / viewport;
	float ao = (hasSSAO == 1) ? texture(ssao, fragCoords).r : 1.0f;
	float alpha = (material.hasAlbedo == 1) ? texture(material.albedoMap, fs_in.texCoords).a * material.opacity : material.opacity;
	if(material.hasAlbedo == 1)
    	albedo = pow(texture(material.albedoMap, fs_in.texCoords).rgb, vec3(2.2f));
	else
		albedo = material.albedo;
	if(material.hasMetallicRough == 1)
	{
    	metallic = texture(material.metallicRoughMap, fs_in.texCoords).b;
    	roughness = texture(material.metallicRoughMap, fs_in.texCoords).g;
	}
	else
	{
		metallic = material.metallic;
		roughness = material.roughness;
	}

	vec3 N;
	if(material.hasNormal == 1)
    	N = getNormalFromMap();
	else
		N = fs_in.normal;
    vec3 V = normalize(cam.viewPos - fs_in.fragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lightCount; ++i) 
    {
		// calculate per-light radiance
        vec3 L = normalize(light[i].position - fs_in.fragPos);
        vec3 H = normalize(V + L);
        float distance = length(light[i].position - fs_in.fragPos);
        float attenuation;
		if(light[i].type == 0)
			attenuation = 1.0f / (light[i].kc + light[i].kl * distance + light[i].kq * (distance * distance));
		else if(light[i].type == 2)
			attenuation = 1.0f / (distance * distance);
		else
			attenuation = 1.0f;
        vec3 radiance = light[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;

		// multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

		// calculate shadow
		float shadow;
		float theta;
		float intensity;
		vec3 lightDir;

		if(light[i].type == 0)
			lightDir = normalize(fs_in.fragPos - light[i].position);
		else if(light[i].type == 1)
			lightDir = light[i].direction;
		else if(light[i].type == 2)
		{
			lightDir = normalize(fs_in.fragPos - light[i].position);
			theta = dot(lightDir, normalize(light[i].direction));
			float epsilon = light[i].cutOff - light[i].outerCutOff;
			intensity = clamp((theta - light[i].outerCutOff) / epsilon, 0.0f, 1.0f);
		}

		if(shadowOn == 1 && light[i].type != 0)
			shadow = calculateShadow(light[i].lightSpaceMatrix * vec4(fs_in.fragPos, 1.0f), lightDir, i);
		else if(shadowOn == 1 && light[i].type == 0)
			shadow = calculateOmniShadow(fs_in.fragPos, light[i].position, i);
		else if(shadowOn == 0)
			shadow = 0.0f;

		// add to outgoing radiance Lo
		if(light[i].type == 2 && theta > light[i].outerCutOff)
        	Lo += (kD * albedo / PI + specular) * radiance * NdotL * (1.0f - shadow) * intensity;
		else if(light[i].type != 2)
        	Lo += (kD * albedo / PI + specular) * radiance * NdotL * (1.0f - shadow);
    }

	vec3 ambient;
	if(IBL == 1)
	{
		vec3 R = reflect(-V, N);
		vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;
		vec3 irradiance = texture(irradianceMap, N).rgb;
		vec3 diffuse = irradiance * albedo;

		const float MAX_REFLECTION_LOD = 4.0;
		vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
		vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
		vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

		ambient = (kD * diffuse + specular * metallic) * ao * 0.33;
	}
	else if(IBL == 0)
	{
		ambient = albedo * ao * 0.33;
	}
    vec3 color = ambient + Lo;
    fragColor = vec4(color, alpha);

	// get brightness
	vec3 emission = material.emissiveColor;
	if(material.hasEmission == 1)
		emission = texture(material.emissionMap, fs_in.texCoords).rgb;
	emission *= material.emissionIntensity;
	float brightness = dot(fragColor.rgb + emission, vec3(0.2126f, 0.7152f, 0.0722f));

	// bright color
	if(brightness > 1.0f)
	{
		brightColor = vec4(fragColor.rgb + emission, alpha);
		if(material.emissionIntensity == 0.0f && brightness > 5.0f)
			brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
		brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
