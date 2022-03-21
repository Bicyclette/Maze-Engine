#version 460 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
    color = texture(image, texCoords);

    // bright color
    float brightness = dot(color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        brightColor = color;
    else
        brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
