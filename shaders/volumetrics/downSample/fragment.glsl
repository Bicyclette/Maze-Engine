#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D fragWorldPos;

void main()
{
	vec2 texelSize = 2.0f / textureSize(fragWorldPos, 0);
	
	// orientation
	vec2 N = vec2(0.0f, 1.0f);
	vec2 S = vec2(0.0f, -1.0f);
	vec2 E = vec2(1.0f, 0.0f);
	vec2 W = vec2(-1.0f, 0.0f);

    color = texture(fragWorldPos, texCoords + texelSize * (S+W));
}
