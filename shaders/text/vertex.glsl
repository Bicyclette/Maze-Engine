#version 460 core

layout (location = 0) in vec4 vertex; // vec2 pos & vec2 tex

out vec2 texCoords;

uniform float zIndex;
uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(vertex.xy, zIndex, 1.0f);
    texCoords = vertex.zw;
}
