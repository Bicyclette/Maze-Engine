#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 174) out;

in VS_OUT 
{
	vec4 direction;
	vec4 right;
	vec4 up;
	float cutOff;
} gs_in[];

void main()
{
	vec4 direction = gs_in[0].direction;
	vec4 right = gs_in[0].right;
	vec4 up = gs_in[0].up;
	float cutOff = gs_in[0].cutOff;

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 5.0f * direction;
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position + 5.0f * direction;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 4.5f * direction + 0.25f * up;
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position + 5.0f * direction;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 4.5f * direction - 0.25f * up;
	EmitVertex();
	EndPrimitive();

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 20.0f * direction + 20.0f * (sin(cutOff) * up);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 20.0f * direction - 20.0f * (sin(cutOff) * up);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 20.0f * direction + 20.0f * (sin(cutOff) * right);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + 20.0f * direction - 20.0f * (sin(cutOff) * right);
	EmitVertex();
	EndPrimitive();

	for(int i = 0; i < 360; i+=9)
	{
		gl_Position = gl_in[0].gl_Position + 10.0f * direction + 10.0f * (sin(cutOff) * (cos(radians(i)) * right + sin(radians(i)) * up));
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + 10.0f * direction + 10.0f * (sin(cutOff) * (cos(radians(i + 9)) * right + sin(radians(i + 9)) * up));
		EmitVertex();
		EndPrimitive();
		
		gl_Position = gl_in[0].gl_Position + 20.0f * direction + 20.0f * (sin(cutOff) * (cos(radians(i)) * right + sin(radians(i)) * up));
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + 20.0f * direction + 20.0f * (sin(cutOff) * (cos(radians(i + 9)) * right + sin(radians(i + 9)) * up));
		EmitVertex();
		EndPrimitive();
	}
}
