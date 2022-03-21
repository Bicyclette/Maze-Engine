#version 460 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

in vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, texCoords).r);
    color = vec4(textColor, 1.0f) * sampled;

    // bright color
    float brightness = dot(color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        brightColor = color;
    else
        brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
