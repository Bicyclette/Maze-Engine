#version 460 core

out vec4 fragColor;

const int MAX_KERNEL_SIZE = 128;

uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D noiseTexture;

uniform float radius;
uniform float bias;
uniform int kernelSize;
uniform vec3 samples[MAX_KERNEL_SIZE];
uniform mat4 projection;

uniform float screenWidth;
uniform float screenHeight;

in vec2 texCoords;

void main()
{
	const vec2 noiseScale = vec2(screenWidth / 4.0f, screenHeight / 4.0f);

	vec3 fragPos = texture(positionBuffer, texCoords).xyz;
	vec3 normal = texture(normalBuffer, texCoords).xyz;
	vec3 randomVec = texture(noiseTexture, texCoords * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0f;
	for(int i = 0; i < kernelSize; ++i)
	{
		// get sample position
		vec3 samplePos = TBN * samples[i]; // from tangent to view-space
		samplePos = fragPos + samplePos * radius;

		vec4 offset = vec4(samplePos, 1.0f);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5f + 0.5f;

		float sampleDepth = texture(positionBuffer, offset.xy).z;

		float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(fragPos.z - sampleDepth));
		occlusion += ((sampleDepth >= samplePos.z + bias) ? 1.0f : 0.0f) * rangeCheck;
	}

	occlusion = 1.0f - (occlusion / kernelSize);
	occlusion = pow(occlusion, 2.0f);

	fragColor = vec4(vec3(occlusion), 1.0f);
}
