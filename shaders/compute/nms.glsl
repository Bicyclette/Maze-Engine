#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

int getOrientation(vec2 gradient)
{
	if (gradient[1] < 0.0f)
	{
		gradient *= -1.0f;
	}
	gradient = gradient / (sqrt(gradient[0] * gradient[0] + gradient[1] * gradient[1]));

	vec2 v[4];
	v[0] = vec2(1.0, 0.0);							// 0
	v[1] = vec2(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0);	// Pi/4
	v[2] = vec2(0.0, 1.0);							// Pi/2
	v[3] = vec2(-sqrt(2.0) / 2.0, sqrt(2.0) / 2.0);	// 3Pi/4

	float f0 = abs(dot(gradient, v[0]));
	float f1 = abs(dot(gradient, v[1]));
	float f2 = abs(dot(gradient, v[2]));
	float f3 = abs(dot(gradient, v[3]));
	float fmax = max(max(max(f3, f2), f1), f0);

	int orientation;
	if (fmax == f0) { orientation = 0; }
	else if (fmax == f1) { orientation = 1; }
	else if (fmax == f2) { orientation = 2; }
	else if (fmax == f3) { orientation = 3; }
	return orientation;
}

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID);
	
	vec4 G_00 = imageLoad(image_in, coords + ivec2(-1, -1));
	vec4 G_01 = imageLoad(image_in, coords + ivec2(0, -1));
	vec4 G_02 = imageLoad(image_in, coords + ivec2(1, -1));
	vec4 G_10 = imageLoad(image_in, coords + ivec2(-1, 0));
    vec4 G_11 = imageLoad(image_in, coords);
	vec4 G_12 = imageLoad(image_in, coords + ivec2(1, 0));
	vec4 G_20 = imageLoad(image_in, coords + ivec2(-1, 1));
	vec4 G_21 = imageLoad(image_in, coords + ivec2(0, 1));
	vec4 G_22 = imageLoad(image_in, coords + ivec2(1, 1));

	int orientation = getOrientation(G_11.xy);
	float color;
	if (orientation == 0)
	{
		if (G_11.z > G_10.z && G_11.z > G_12.z) {
			color = G_11.z;
		}
		else {
			color = 0.0f;
		}
	}
	else if (orientation == 1)
	{
		if (G_11.z > G_02.z && G_11.z > G_20.z) {
			color = G_11.z;
		}
		else {
			color = 0.0f;
		}
	}
	else if (orientation == 2)
	{
		if (G_11.z > G_01.z && G_11.z > G_21.z) {
			color = G_11.z;
		}
		else {
			color = 0.0f;
		}
	}
	else if (orientation == 3)
	{
		if (G_11.z > G_00.z && G_11.z > G_22.z) {
			color = G_11.z;
		}
		else {
			color = 0.0;
		}
	}

	// final output
	imageStore(image_out, coords, vec4(color, color, color, 1.0f));
}
