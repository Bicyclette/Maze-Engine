#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

uniform int blurSize;
uniform float sigma;
uniform int direction;

const float PI = 3.14159265f;

void main()
{
    int halfSize = blurSize / 2;
    ivec2 blurDirection = (direction == 0) ? ivec2(1, 0) : ivec2(0, 1);
    ivec2 coords = ivec2(gl_GlobalInvocationID);

    const float gaussLeft = 1.0f / sqrt(2.0f * PI * sigma);
    const float twoSigmaSquared = 2.0f * sigma * sigma;

    vec4 color = vec4(0.0f);
    float sum = 0.0f;
    for(int i = -halfSize; i < halfSize; ++i)
    {
        ivec2 uv = coords + blurDirection * i;
        float g = gaussLeft * exp(-(i*i)/twoSigmaSquared);
        color.rgb += imageLoad(image_in, uv).rgb * g;
        sum += g;
    }
    color.rgb /= sum;
    color.a = 1.0f;
    imageStore(image_out, coords, color);
}
