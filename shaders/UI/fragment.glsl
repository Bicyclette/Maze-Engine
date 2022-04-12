#version 460 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

in vec2 texCoords;

uniform sampler2D image;
uniform vec4 bkg_color;
uniform bool use_bkg_img;
uniform float bloom_strength;

void main()
{
    if(use_bkg_img)
        color = texture(image, texCoords);
    else
        color = bkg_color;

    // bright color
    float brightness = dot(color.rgb*bloom_strength, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        brightColor = color;
    else
        brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
