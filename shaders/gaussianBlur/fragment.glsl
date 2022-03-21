#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D image;
uniform int blurSize;
uniform float sigma;
uniform int direction;

const float PI = 3.14159265;

void main()
{
	vec2 texelSize = 1.0f / textureSize(image, 0);
	int halfSize = blurSize / 2;
	vec2 blurDirection = (direction == 0) ? vec2(1.0f, 0.0f) : vec2(0.0f, 1.0f);
	
	const float gaussLeft = 1.0f / sqrt(2.0f * PI * sigma);
	const float twoSigmaSquared = 2.0f * sigma * sigma;

	float sum = 0.0f;
	for(int i = -halfSize; i < halfSize+1; ++i)
	{
		vec2 uv = texCoords + texelSize * blurDirection * i;
		float g = gaussLeft * exp(-(i*i)/twoSigmaSquared);
		color += texture(image, uv) * g;
		sum += g;
	}
	color /= sum;
}
