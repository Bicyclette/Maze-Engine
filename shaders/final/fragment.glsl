#version 460 core

out vec4 fragColor;

uniform sampler2D scene;
uniform sampler2D bloom;
uniform int bloomEffect;

in vec2 texCoords;

vec4 gammaCorrection(vec4 c)
{
	float gamma = 1.0/2.2;
	return vec4(pow(c.rgb, vec3(gamma)), c.a);
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

	// reinhard tone mapping
	vec3 mapped = color / (color + vec3(1.0));

	// vignette effect
	vec3 vignette = vec3(1.0f);
	float dist = distance(texCoords, vec2(0.5, 0.5));
	vignette = vec3(1.0f) - (vignette / (1.0f / dist));
	mapped *= vignette;

	// gamma correction
	fragColor = gammaCorrection(vec4(mapped, 1.0));

	// show scene data (if one GBufferFBO's attachment was sent on scene uniform) or AO data
	//fragColor = texture(scene, texCoords);
	//fragColor = vec4(vec3(texture(scene, texCoords).r), 1.0f);
	//fragColor = texture(bloom, texCoords);
}
