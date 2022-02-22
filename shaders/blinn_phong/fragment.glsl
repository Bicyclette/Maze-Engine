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
	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;
	mat4 lightSpaceMatrix;
};

struct Material
{
	vec3 color_diffuse;
	vec3 color_specular;
	vec3 color_ambient;
	float opacity;
	float shininess;
	sampler2D diffuse;
	int hasDiffuse;
	sampler2D specular;
	int hasSpecular;
	sampler2D normal;
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
	mat3 TBN;
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

float linearizeDepth(float depth)
{
	float near = 0.1f;
	float far = 100.0f;
	float ndc = depth * 2.0f - 1.0f;
	float z = (2.0f * near * far) / (far + near - ndc * (far - near));
	return z / far;
}

vec3 calculateDiffuse(vec3 lightDir, vec3 diffuseStrength, vec3 objColor)
{
	vec3 norm;
	if(material.hasNormal == 1)
	{
		norm = texture(material.normal, fs_in.texCoords).rgb;
		norm = normalize(norm * 2.0f - 1.0f);
		lightDir = fs_in.TBN * lightDir;
	}
	else
		norm = normalize(fs_in.normal);
	float diff = max(dot(norm, -lightDir), 0.0f);
	return diffuseStrength * diff * objColor;
}

vec3 calculateSpecular(vec3 viewPos, vec3 lightDir, vec3 specularStrength, vec3 objColor)
{
	vec3 fragPos;
	vec3 norm;
	if(material.hasNormal == 1)
	{
		fragPos = fs_in.TBN * fs_in.fragPos;
		norm = texture(material.normal, fs_in.texCoords).rgb;
		norm = normalize(norm * 2.0f - 1.0f);
		lightDir = fs_in.TBN * lightDir;
	}
	else
	{
		fragPos = fs_in.fragPos;
		norm = normalize(fs_in.normal);
	}
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(-lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), material.shininess);
	return specularStrength * spec * objColor;
}

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

void main()
{
	// early discard
	if(material.nbTextures > 0)
	{
		if(texture(material.diffuse, fs_in.texCoords).a == 0.0f)
			discard;
	}

	// start
	vec2 fragCoords = gl_FragCoord.xy / viewport;
	float ao = (hasSSAO == 1) ? texture(ssao, fragCoords).r : 1.0f;
	vec3 color;

	for(int l = 0; l < lightCount; ++l)
	{
		vec3 lightPos = light[l].position;
		vec3 fragPos = fs_in.fragPos;
		vec3 viewPos = cam.viewPos;

		if(material.hasNormal == 1)
		{
			lightPos = fs_in.TBN * light[l].position;
			fragPos = fs_in.TBN * fs_in.fragPos;
			viewPos = fs_in.TBN * cam.viewPos;
		}

		vec3 lightDir;
		float dist = length(lightPos - fragPos);
		float attenuation;
		float theta;
		float epsilon;
		float intensity;

		if(light[l].type == 0)
		{
			lightDir = normalize(fs_in.fragPos - light[l].position);
			attenuation = 1.0f / (light[l].kc + light[l].kl * dist + light[l].kq * (dist * dist));
		}
		else if(light[l].type == 1)
		{
			lightDir = light[l].direction;
		}
		else if(light[l].type == 2)
		{
			lightDir = normalize(fs_in.fragPos - light[l].position);
			theta = dot(lightDir, normalize(light[l].direction));
			epsilon = light[l].cutOff - light[l].outerCutOff;
			intensity = clamp((theta - light[l].outerCutOff) / epsilon, 0.0f, 1.0f);
		}

		// calculate shadow
		float shadow;
		if(shadowOn == 1 && light[l].type != 0)
			shadow = calculateShadow(light[l].lightSpaceMatrix * vec4(fs_in.fragPos, 1.0f), lightDir, l);
		else if(shadowOn == 1 && light[l].type == 0)
			shadow = calculateOmniShadow(fs_in.fragPos, light[l].position, l);
		else if(shadowOn == 0)
			shadow = 0.0f;

		if(material.nbTextures > 0)
		{
			// ambient
			vec3 ambient = ao * light[l].ambientStrength * texture(material.diffuse, fs_in.texCoords).rgb;

			if(light[l].type == 2 && theta > light[l].outerCutOff)
			{
				// diffuse
				vec3 diffuse = calculateDiffuse(lightDir, light[l].diffuseStrength, texture(material.diffuse, fs_in.texCoords).rgb);

				// specular
				vec3 specular;
				if(material.hasSpecular == 1)
					specular = calculateSpecular(viewPos, lightDir, light[l].specularStrength, texture(material.specular, fs_in.texCoords).rgb);
				else
					specular = calculateSpecular(viewPos, lightDir, light[l].specularStrength, material.color_specular);
				
				diffuse = diffuse * intensity * (1.0 - shadow);
				specular = specular * intensity * (1.0 - shadow);
			
				// putting it all together
				color += ambient + diffuse + specular;
			}
			else if(light[l].type == 2)
			{
				color += ambient;
			}
			else
			{
				// diffuse
				vec3 diffuse = calculateDiffuse(lightDir, light[l].diffuseStrength, texture(material.diffuse, fs_in.texCoords).rgb) * (1.0 - shadow);

				// specular
				vec3 specular;
				if(material.hasSpecular == 1)
					specular = calculateSpecular(viewPos, lightDir, light[l].specularStrength, texture(material.specular, fs_in.texCoords).rgb) * (1.0 - shadow);
				else
					specular = calculateSpecular(viewPos, lightDir, light[l].specularStrength, material.color_specular) * (1.0 - shadow);
				
				// putting it all together
				if(light[l].type == 0)
					color += (ambient + diffuse + specular) * attenuation;
				else
					color += (ambient + diffuse + specular);
			}
		}

		else
		{
			// ambient
			vec3 ambient = ao * light[l].ambientStrength * material.color_ambient;

			if(light[l].type == 2 && theta > light[l].outerCutOff)
			{
				// diffuse
				vec3 diffuse = calculateDiffuse(lightDir, light[l].diffuseStrength, material.color_diffuse) * (1.0 - shadow);

				// specular
				vec3 specular = calculateSpecular(viewPos, lightDir, light[l].specularStrength, material.color_specular) * (1.0 - shadow);
				
				diffuse = diffuse * intensity;
				specular = specular * intensity;
			
				// putting it all together
				color += ambient + diffuse + specular;
			}
			else if(light[l].type == 2)
			{
				color += ambient;
			}
			else
			{
				// diffuse
				vec3 diffuse = calculateDiffuse(lightDir, light[l].diffuseStrength, material.color_diffuse) * (1.0 - shadow);

				// specular
				vec3 specular = calculateSpecular(viewPos, lightDir, light[l].specularStrength, material.color_specular) * (1.0 - shadow);

				// putting it all together
				if(light[l].type == 0)
					color += (ambient + diffuse + specular) * attenuation;
				else
					color += (ambient + diffuse + specular);
			}
		}
	}

	float alpha = (material.hasDiffuse == 1) ? texture(material.diffuse, fs_in.texCoords).a * material.opacity : material.opacity;
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
