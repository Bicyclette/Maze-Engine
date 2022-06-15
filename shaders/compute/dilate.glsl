#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    
    float color = 0.0f;

    float e00 = imageLoad(image_in, coords + ivec2(-1, -1)).x;
    float e01 = imageLoad(image_in, coords + ivec2(0, -1)).x;
    float e02 = imageLoad(image_in, coords + ivec2(1, -1)).x;
    float e10 = imageLoad(image_in, coords + ivec2(-1, 0)).x;
    float e11 = imageLoad(image_in, coords + ivec2(0, 0)).x;
    float e12 = imageLoad(image_in, coords + ivec2(1, 0)).x;
    float e20 = imageLoad(image_in, coords + ivec2(-1, 1)).x;
    float e21 = imageLoad(image_in, coords + ivec2(0, 1)).x;
    float e22 = imageLoad(image_in, coords + ivec2(1, 1)).x;
    
    float sum = e00 + e01 + e02 + e10 + e11 + e12 + e20 + e21 + e22;
    if(sum > 0.0f) {
        color = 1.0f;
    }

    imageStore(image_out, coords, vec4(color, color, color, 1.0f));
}
