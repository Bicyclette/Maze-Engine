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

const float arrow_length = 2.5f;
const float first_circle_dist = 5.0f;
const float second_circle_dist = 10.0f;

void main()
{
	vec4 direction = gs_in[0].direction;
	vec4 right = gs_in[0].right;
	vec4 up = gs_in[0].up;
	float cutOff = gs_in[0].cutOff;

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + arrow_length * direction;
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position + arrow_length * direction;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + (arrow_length - 0.5f) * direction + 0.25f * up;
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position + arrow_length * direction;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + (arrow_length - 0.5f) * direction - 0.25f * up;
	EmitVertex();
	EndPrimitive();

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + second_circle_dist * direction + second_circle_dist * (sin(cutOff) * up);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + second_circle_dist * direction - second_circle_dist * (sin(cutOff) * up);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + second_circle_dist * direction + second_circle_dist * (sin(cutOff) * right);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + second_circle_dist * direction - second_circle_dist * (sin(cutOff) * right);
	EmitVertex();
	EndPrimitive();

	for(int i = 0; i < 360; i+=9)
	{
		gl_Position = gl_in[0].gl_Position + first_circle_dist * direction + first_circle_dist * (sin(cutOff) * (cos(radians(i)) * right + sin(radians(i)) * up));
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + first_circle_dist * direction + first_circle_dist * (sin(cutOff) * (cos(radians(i + 9)) * right + sin(radians(i + 9)) * up));
		EmitVertex();
		EndPrimitive();
		
		gl_Position = gl_in[0].gl_Position + second_circle_dist * direction + second_circle_dist * (sin(cutOff) * (cos(radians(i)) * right + sin(radians(i)) * up));
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + second_circle_dist * direction + second_circle_dist * (sin(cutOff) * (cos(radians(i + 9)) * right + sin(radians(i + 9)) * up));
		EmitVertex();
		EndPrimitive();
	}
}
