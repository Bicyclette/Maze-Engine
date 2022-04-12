#version 460 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 ui_mask;

in vec2 texCoords;

uniform sampler2D ui;
uniform sampler2D uiBloom;
uniform int tone_mapping; // 0 = Reinhard, 1 = ACES, 2 = OFF

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
    // user interface
    vec4 ui_color = texture(ui, texCoords);
    vec4 ui_bloom = texture(uiBloom, texCoords);
    vec4 userInterface = ui_color + ui_bloom;

    // tone mapping
	vec3 color;
	if(tone_mapping == 0)
		color = reinhard(userInterface.rgb);
	else if(tone_mapping == 1)
		color = ACES_tone_mapping(userInterface.rgb);
	else
		color = userInterface.rgb;

	// gamma correction
	fragColor = gammaCorrection(vec4(color, 1.0f));

	// mask
	if(ui_color.a != 0.0f)
		ui_mask = vec4(1.0f);
	else
		ui_mask = vec4(0.0f);
}
