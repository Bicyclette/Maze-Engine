#version 460 core

layout (triangles) in;
layout (triangle_strip,max_vertices = 18) out;

uniform bool omniDepthRendering;
uniform mat4 omnilightViews[6];

in VS_OUT
{
	vec2 texCoords;
}gs_in[];

out GS_OUT
{
	vec2 texCoords;
	vec4 fragPos;
}gs_out;

void main()
{
	if(omniDepthRendering)
	{
		for(int f = 0; f < 6; ++f)
		{
			gl_Layer = f;
			for(int v = 0; v < 3; ++v)
			{
				gs_out.texCoords = gs_in[v].texCoords;
				gs_out.fragPos = gl_in[v].gl_Position;
				gl_Position = omnilightViews[f] * gs_out.fragPos;
				EmitVertex();
			}
			EndPrimitive();
		}
	}
	else
	{
		gl_Position = gl_in[0].gl_Position;
		gs_out.texCoords = gs_in[0].texCoords;
		gs_out.fragPos = gl_in[0].gl_Position;
		EmitVertex();
		
		gl_Position = gl_in[1].gl_Position;
		gs_out.texCoords = gs_in[1].texCoords;
		gs_out.fragPos = gl_in[1].gl_Position;
		EmitVertex();
		
		gl_Position = gl_in[2].gl_Position;
		gs_out.texCoords = gs_in[2].texCoords;
		gs_out.fragPos = gl_in[2].gl_Position;
		EmitVertex();
		
		EndPrimitive();
	}
}
