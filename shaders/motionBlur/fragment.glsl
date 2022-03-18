#version 460 core

layout (location = 0) out vec4 color;

in VS_OUT
{
    vec2 texCoords;
} fs_in;

uniform sampler2D worldPos;
uniform mat4 prev_MVP;
uniform mat4 curr_MVP;

void main()
{
    vec4 world_coords = texture(worldPos, fs_in.texCoords);
    vec4 prev_pos = prev_MVP * world_coords;
    prev_pos /= prev_pos.w;
    vec4 curr_pos = curr_MVP * world_coords;
    curr_pos /= curr_pos.w;
    color = prev_pos - curr_pos;
    color.a = 1.0f;
}
