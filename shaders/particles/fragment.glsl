#version 460 core

out vec4 color;

uniform sampler2D particle;

in GS_OUT
{
	vec2 texCoords;
	float lifetimeRatio;
} fs_in;

void main()
{
	color = texture(particle, fs_in.texCoords);
	//color.a *= min(0.5f, 1.0f - fs_in.lifetimeRatio);
	color.a *= min(- log(fs_in.lifetimeRatio), 0.5f);
	//color.a *= max(0.5f, fs_in.lifetimeRatio) * 0.5f;
	//color.a *= max(0.5f, fs_in.lifetimeRatio * 0.75f);
	//color.a *= fs_in.lifetimeRatio * 0.75f;
	//color.a *= fs_in.lifetimeRatio;
	//color.a *= 0.25f;
}
