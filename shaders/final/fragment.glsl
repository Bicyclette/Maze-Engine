#version 460 core

out vec4 fragColor;

uniform sampler2D scene;
uniform sampler2D bloom;
uniform int bloomEffect;
uniform int tone_mapping; // 0 = Reinhard, 1 = ACES

in vec2 texCoords;

vec4 gammaCorrection(vec4 c)
{
	float gamma = 1.0/2.2;
	return vec4(pow(c.rgb, vec3(gamma)), c.a);
}

vec3 reinhard(vec3 data)
{
	return data / (data + 1.0f);
}

vec3 ACES_tone_mapping(vec3 data)
{
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return clamp((data*(a*data+b))/(data*(c*data+d)+e),0.0f, 1.0f);
}

void main()
{
	vec3 sceneColor = texture(scene, texCoords).rgb;
	vec3 bloomColor = vec3(0.0f);
	if(bloomEffect == 1)
	{
		bloomColor = texture(bloom, texCoords).rgb;
	}

	// gather colors
	vec3 color = sceneColor + bloomColor;

	// tone mapping
	if(tone_mapping == 0)
		color = reinhard(color);
	else if(tone_mapping == 1)
		color = ACES_tone_mapping(color);

	// vignette effect
	vec3 vignette = vec3(1.0f);
	float dist = distance(texCoords, vec2(0.5, 0.5));
	vignette = vec3(1.0f) - (vignette / (1.0f / dist));
	color *= vignette;

	// gamma correction
	fragColor = gammaCorrection(vec4(color, 1.0));

	// show scene data (if one GBufferFBO's attachment was sent on scene uniform) or AO data
	//fragColor = texture(scene, texCoords);
	//fragColor = vec4(vec3(texture(scene, texCoords).r), 1.0f);
	//fragColor = texture(bloom, texCoords);
}
