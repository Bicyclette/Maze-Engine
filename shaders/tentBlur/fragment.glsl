#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
	vec2 texelSize = 1.0f / textureSize(image, 0);
	color.rgb += texture(image, texCoords + vec2(-1.0f, 1.0f) * texelSize).rgb;
	color.rgb += texture(image, texCoords + vec2(0.0f, 1.0f) * texelSize).rgb * 2.0f;
	color.rgb += texture(image, texCoords + vec2(1.0f, 1.0f) * texelSize).rgb;
	color.rgb += texture(image, texCoords + vec2(-1.0f, 0.0f) * texelSize).rgb * 2.0f;
	color.rgb += texture(image, texCoords + vec2(0.0f, 0.0f) * texelSize).rgb * 4.0f;
	color.rgb += texture(image, texCoords + vec2(1.0f, 0.0f) * texelSize).rgb * 2.0f;
	color.rgb += texture(image, texCoords + vec2(-1.0f, -1.0f) * texelSize).rgb;
	color.rgb += texture(image, texCoords + vec2(0.0f, -1.0f) * texelSize).rgb * 2.0f;
	color.rgb += texture(image, texCoords + vec2(1.0f, -1.0f) * texelSize).rgb;
	color.rgb /= 16.0f;
	color.a = 1.0f;
}
