#version 460 core

out vec4 fragColor;

uniform sampler2D scene;
uniform sampler2D ui;
uniform sampler2D ui_mask;

in vec2 texCoords;

void main()
{
	vec4 scene_sample = texture(scene, texCoords);
	vec4 ui_sample = texture(ui, texCoords);
	float mask = texture(ui_mask, texCoords).r;
	
	if(mask == 1.0f)
		fragColor = ui_sample;
	else
		fragColor = ui_sample + scene_sample;
	fragColor.a = 1.0f;
}
