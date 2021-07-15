#version 460 core

out vec4 fragColor;
in vec3 localPos;

uniform sampler2D equirectangular_env_map;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 samplerSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = samplerSphericalMap(normalize(localPos));
	vec3 color = texture(equirectangular_env_map, uv).rgb;

	fragColor = vec4(color, 1.0f);
}
