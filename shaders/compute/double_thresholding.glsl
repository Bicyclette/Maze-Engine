#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

uniform float low_thr;
uniform float high_thr;

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    float G = imageLoad(image_in, coords).x;
    float color;
    if(G < low_thr) {
        color = 0.0f;
    }
    else if(G >= low_thr && G <= high_thr) {
        color = 0.5f;
    }
    else {
        color = 1.0f;
    }
    imageStore(image_out, coords, vec4(color, color, color, 1.0f));
}
