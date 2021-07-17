#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

uniform float maxLifetime;
uniform float numImagesX;
uniform float numImagesY;

in VS_OUT
{
	vec4 up;
	vec4 right;
	float lifetime;
} gs_in[];

out GS_OUT 
{
	vec2 texCoords;
	float lifetimeRatio;
} gs_out;

void main()
{
	float minSize = 0.25f;
	float maxSize = 1.0f;

	vec4 up = gs_in[0].up;
	vec4 right = gs_in[0].right;
	float lifetime = gs_in[0].lifetime;

	float xStride = 1.0f / numImagesX;
	float yStride = 1.0f / numImagesY;

	float ratio = lifetime / maxLifetime;
	float size = mix(minSize, maxSize, ratio);

	highp int img = int(ratio * numImagesX * numImagesY);
	int x = img % int(numImagesX);
	int y = img / int(numImagesX);

	float cLeft = x * xStride;
	float cRight = cLeft + xStride;
	float cBottom = (int(numImagesY) - y) * yStride;
	float cTop = cBottom - yStride;

	gl_Position = gl_in[0].gl_Position + size * (up - right);
	gs_out.texCoords = vec2(cLeft, cTop);
	gs_out.lifetimeRatio = ratio;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + size * (- up - right);
	gs_out.texCoords = vec2(cLeft, cBottom);
	gs_out.lifetimeRatio = ratio;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + size * (- up + right);
	gs_out.texCoords = vec2(cRight, cBottom);
	gs_out.lifetimeRatio = ratio;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + size * (up - right);
	gs_out.texCoords = vec2(cLeft, cTop);
	gs_out.lifetimeRatio = ratio;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + size * (- up + right);
	gs_out.texCoords = vec2(cRight, cBottom);
	gs_out.lifetimeRatio = ratio;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + size * (up + right);
	gs_out.texCoords = vec2(cRight, cTop);
	gs_out.lifetimeRatio = ratio;
	EmitVertex();
	EndPrimitive();
}
