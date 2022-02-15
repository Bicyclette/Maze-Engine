#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D low_res;
uniform sampler2D high_res;

void main()
{
	vec3 merge = texture(low_res, texCoords).rgb + texture(high_res, texCoords).rgb;
	color = vec4(merge, 1.0f);
}
