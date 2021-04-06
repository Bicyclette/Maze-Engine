#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

out GS_OUT 
{
	vec2 texCoords;
} gs_out;

void main()
{
	float w = gl_in[0].gl_Position.w;

	gl_Position = gl_in[0].gl_Position + vec4(-0.035f, 0.0625f, 0.0f, 0.0f) * w;
	gs_out.texCoords = vec2(0.0f, 1.0f);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.035f, -0.0625f, 0.0f, 0.0f) * w;
	gs_out.texCoords = vec2(0.0f, 0.0f);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.035f, -0.0625f, 0.0f, 0.0f) * w;
	gs_out.texCoords = vec2(1.0f, 0.0f);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.035f, 0.0625f, 0.0f, 0.0f) * w;
	gs_out.texCoords = vec2(1.0f, 1.0f);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.035f, 0.0625f, 0.0f, 0.0f) * w;
	gs_out.texCoords = vec2(0.0f, 1.0f);
	EmitVertex();
	EndPrimitive();
}
