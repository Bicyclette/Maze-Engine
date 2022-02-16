#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = ) out;

uniform int num_arcs;
uniform int step;

uniform float arc_amplitude[5];
uniform float arc_thickness[5];

void main()
{
	float w = gl_in[0].gl_Position.w;

	gl_Position = gl_in[0].gl_Position + vec4() * w;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4() * w;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4() * w;
	EmitVertex();
	EndPrimitive();
}
