#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 240) out;

in VS_OUT
{
	vec4 up;
	vec4 right;
	vec4 front;
} gs_in[];

void main()
{
	vec4 up = gs_in[0].up;
	vec4 right = gs_in[0].right;
	vec4 front = gs_in[0].front;

	for(int i = 0; i < 360; i+=9)
	{
		float angle = radians(i);
		float angle2 = radians(i + 9);

		gl_Position = gl_in[0].gl_Position + cos(angle) * right + sin(angle) * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + cos(angle2) * right + sin(angle2) * up;
		EmitVertex();
		EndPrimitive();
		
		gl_Position = gl_in[0].gl_Position + cos(angle) * right + sin(angle) * front;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + cos(angle2) * right + sin(angle2) * front;
		EmitVertex();
		EndPrimitive();
		
		gl_Position = gl_in[0].gl_Position + cos(angle) * front + sin(angle) * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + cos(angle2) * front + sin(angle2) * up;
		EmitVertex();
		EndPrimitive();
	}
}
