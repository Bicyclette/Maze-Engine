#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 44) out;

in VS_OUT
{
	vec4 direction;
	vec4 right;
	vec4 up;
} gs_in[];

uniform float boxDim;

void main()
{
	vec4 direction = gs_in[0].direction;
	vec4 right = gs_in[0].right;
	vec4 up = gs_in[0].up;
	
	// first ray
	// ---------
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + (2.5f * direction);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position - (2.5f * direction);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position + (2.5f * direction);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + (2.0f * direction) + 0.25f * up;
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position + (2.5f * direction);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + (2.0f * direction) - 0.25f * up;
	EmitVertex();
	EndPrimitive();

	if(boxDim > 0.0f)
	{
		// ortho box
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up;
		EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up;
		EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up + 100.0f * direction;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up + 100.0f * direction;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position + boxDim * right + boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position - boxDim * right + boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up;
		EmitVertex();
		EndPrimitive();
	
		gl_Position = gl_in[0].gl_Position - boxDim * right - boxDim * up;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + boxDim * right - boxDim * up;
		EmitVertex();
		EndPrimitive();
	}
}
