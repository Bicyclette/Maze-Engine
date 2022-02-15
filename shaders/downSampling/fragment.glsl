#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
	vec2 texelSize = 1.0f / textureSize(image, 0);
	
	// orientation
	vec2 N = vec2(0.0f, 1.0f);
	vec2 S = vec2(0.0f, -1.0f);
	vec2 E = vec2(1.0f, 0.0f);
	vec2 W = vec2(-1.0f, 0.0f);
	
	// 4x4 box
	vec3 CBox = vec3(0.0f);
	vec3 NWBox = vec3(0.0f);
	vec3 NEBox = vec3(0.0f);
	vec3 SEBox = vec3(0.0f);
	vec3 SWBox = vec3(0.0f);
	
	// get 5 areas
	vec2 uv = texCoords;
	CBox += texture(image, uv + texelSize * (N+W)).rgb;
	CBox += texture(image, uv + texelSize * (N+E)).rgb;
	CBox += texture(image, uv + texelSize * (S+W)).rgb;
	CBox += texture(image, uv + texelSize * (S+E)).rgb;
	CBox /= 4.0f;
	
	uv = texCoords + texelSize * (N+W);
	NWBox += texture(image, uv + texelSize * (N+W)).rgb;
	NWBox += texture(image, uv + texelSize * (N+E)).rgb;
	NWBox += texture(image, uv + texelSize * (S+W)).rgb;
	NWBox += texture(image, uv + texelSize * (S+E)).rgb;
	NWBox /= 4.0f;
	
	uv = texCoords + texelSize * (N+E);
	NEBox += texture(image, uv + texelSize * (N+W)).rgb;
	NEBox += texture(image, uv + texelSize * (N+E)).rgb;
	NEBox += texture(image, uv + texelSize * (S+W)).rgb;
	NEBox += texture(image, uv + texelSize * (S+E)).rgb;
	NEBox /= 4.0f;
	
	uv = texCoords + texelSize * (S+E);
	SEBox += texture(image, uv + texelSize * (N+W)).rgb;
	SEBox += texture(image, uv + texelSize * (N+E)).rgb;
	SEBox += texture(image, uv + texelSize * (S+W)).rgb;
	SEBox += texture(image, uv + texelSize * (S+E)).rgb;
	SEBox /= 4.0f;
	
	uv = texCoords + texelSize * (S+W);
	SWBox += texture(image, uv + texelSize * (N+W)).rgb;
	SWBox += texture(image, uv + texelSize * (N+E)).rgb;
	SWBox += texture(image, uv + texelSize * (S+W)).rgb;
	SWBox += texture(image, uv + texelSize * (S+E)).rgb;
	SWBox /= 4.0f;

	// karis average
	vec3 karisAvg = 0.5 * CBox + 0.125 * (NWBox+NEBox+SEBox+SWBox);
	color = vec4(karisAvg, 1.0f);
}
