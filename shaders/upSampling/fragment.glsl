#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D low_res;
uniform sampler2D high_res;

uniform int merge_to_current_FBO;

void main()
{
    if(merge_to_current_FBO == 1)
        color = texture(low_res, texCoords);
    else
    {
        vec4 colorA = texture(low_res, texCoords);
        vec4 colorB = texture(high_res, texCoords);
	    color = colorA + colorB;
        color.a = 1.0f;
    }
}
