#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D outline;
layout(rgba16f, binding = 1) readonly uniform image2D image_in;
layout(rgba16f, binding = 2) writeonly uniform image2D image_out;

uniform vec3 outline_color;

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    vec3 color;
    vec4 img_sample = imageLoad(image_in, coords);
    float edge = imageLoad(outline, coords).x;
    if(edge > 0.0f)
    {
        color = outline_color * edge;
    }
    else
    {
        color = img_sample.rgb;
    }
    imageStore(image_out, coords, vec4(color, 1.0f));
}
