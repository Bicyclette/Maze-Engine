#version 460 core

out vec4 fragColor;

uniform sampler2D aoInput;

in vec2 texCoords;

void main()
{
	vec2 texelSize = 1.0f / vec2(textureSize(aoInput, 0));
	float result = 0.0f;

	for(int x = -2; x < 2; ++x)
	{
		for(int y = -2; y < 2; ++y)
		{
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(aoInput, texCoords + offset).r;
		}
	}
	fragColor = vec4(vec3(result / 16.0f), 1.0f);
}
