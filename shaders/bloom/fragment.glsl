#version 460 core

out vec4 color;


in vec2 texCoords;

uniform sampler2D image;
uniform int horizontal;

const float weight[18] = float[] 
(
 0.079693,
 0.07812,
 0.073585,
 0.066605,
 0.05793,
 0.048417,
 0.038884,
 0.030007,
 0.022252,
 0.015856,
 0.010857,
 0.007144,
 0.004517,
 0.002744,
 0.001602,
 0.000899,
 0.000484,
 0.000251
 );

void main()
{
	vec2 texelSize = 1.0f / textureSize(image, 0);
	vec3 result = texture(image, texCoords).rgb * weight[0];

	if(horizontal == 1)
	{
		for(int i = 1; i < 18; ++i)
		{
			result += texture(image, texCoords + vec2(texelSize.x * i, 0.0f)).rgb * weight[i];
			result += texture(image, texCoords - vec2(texelSize.x * i, 0.0f)).rgb * weight[i];
		}
	}
	else if(horizontal == 0)
	{
		for(int i = 1; i < 18; ++i)
		{
			result += texture(image, texCoords + vec2(0.0f, texelSize.x * i)).rgb * weight[i];
			result += texture(image, texCoords - vec2(0.0f, texelSize.x * i)).rgb * weight[i];
		}
	}

	color = vec4(result, 1.0f);
}
