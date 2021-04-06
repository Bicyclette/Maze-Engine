#version 460 core

in GS_OUT
{
	vec2 texCoords;
} fs_in;

out vec4 color;

uniform sampler2D icon;

void main()
{
	vec4 texColor = texture(icon, fs_in.texCoords);
	if(texColor.a < 0.1f)
		discard;
	color = texColor;
}
