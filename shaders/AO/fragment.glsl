#version 460 core

out float fragColor;

uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D noiseTexture;

uniform float radius;
uniform float bias;
uniform vec3 samples[64];
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
	for(int i = 0; i < 64; ++i)
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
	occlusion = 1.0f - (occlusion / 64.0f);
	fragColor = pow(occlusion, 1.5f);
}
