#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

mat3 sobelX = mat3(
    vec3(-1, -2, -1),
    vec3(0, 0, 0),
    vec3(1, 2, 1)
);

mat3 sobelY = mat3(
    vec3(1, 0, -1),
    vec3(2, 0, -2),
    vec3(1, 0, -1)
);

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    float GX = 0.0f;
    float GY = 0.0f;
    float G = 0.0f;
    for(int line = -1; line < 2; ++line)
    {
        for(int col = -1; col < 2; ++col)
        {
            vec4 img_sample = imageLoad(image_in, coords + ivec2(col, line));
            // turn to grey
            float grey = dot(img_sample.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
            // compute gradient
            GX += grey * sobelX[col+1][line+1];
            GY += grey * sobelY[col+1][line+1];
        }
    }
    G = sqrt(GX*GX + GY*GY);
    imageStore(image_out, coords, vec4(GX, GY, G, 1.0f));
}
