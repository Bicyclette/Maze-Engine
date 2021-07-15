#version 460 core

out vec4 fragColor;

in vec3 localPos;

uniform samplerCube env_map;

const float PI = 3.14159265359;

void main()
{
	vec3 normal = normalize(localPos);

	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, normal));
	up = normalize(cross(normal, right));

	float sampleDelta = 0.025;
	float numSamples = 0.0;

	for(float phi = 0.0; phi < (2.0 * PI); phi += sampleDelta)
	{
		for(float theta = 0.0; theta < (0.5 * PI); theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

			irradiance += texture(env_map, sampleVec).rgb * cos(theta) * sin(theta);
			numSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(numSamples));

	fragColor = vec4(irradiance, 1.0);
}
