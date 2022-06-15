#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

uniform float weak;
uniform float strong;

void main()
{
    float color = 0.0f;
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    float strength = imageLoad(image_in, coords).x;
    if(strength >= weak - 0.01f && strength <= weak + 0.01f)
    {
        for(int line = -1; line < 2; ++line)
        {
            for(int col = -1; col < 2; ++col)
            {
                if (line == 0 && col == 0) {
                    continue;
                }
                ivec2 shift = ivec2(col, line);
                float str = imageLoad(image_in, coords + shift).x;
                if(str == strong){
                    color = 1.0f;
                    break;
                }
            }
            if(color == strong) {
                break;
            }
        }
    }
    else if(strength >= (strong - 0.01f))
    {
        color = 1.0f;
    }
    imageStore(image_out, coords, vec4(color, color, color, 1.0f));
}
