#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D image;
uniform int kernelSize;
uniform float sigma;
uniform int direction;

const float PI = 3.14159265;

vec3 rgb2xyz(vec3 c)
{
    vec3 tmp;
    tmp.x = ( c.r > 0.04045 ) ? pow( ( c.r + 0.055 ) / 1.055, 2.4 ) : c.r / 12.92;
    tmp.y = ( c.g > 0.04045 ) ? pow( ( c.g + 0.055 ) / 1.055, 2.4 ) : c.g / 12.92,
    tmp.z = ( c.b > 0.04045 ) ? pow( ( c.b + 0.055 ) / 1.055, 2.4 ) : c.b / 12.92;
    const mat3x3 mat = mat3x3(
		0.4124, 0.3576, 0.1805,
        0.2126, 0.7152, 0.0722,
        0.0193, 0.1192, 0.9505 
	);
    return 100.0 * (mat * tmp);
}

vec3 xyz2lab(vec3 c)
{
    vec3 n = c / vec3(95.047, 100, 108.883);
    vec3 v;
    v.x = ( n.x > 0.008856 ) ? pow( n.x, 1.0 / 3.0 ) : ( 7.787 * n.x ) + ( 16.0 / 116.0 );
    v.y = ( n.y > 0.008856 ) ? pow( n.y, 1.0 / 3.0 ) : ( 7.787 * n.y ) + ( 16.0 / 116.0 );
    v.z = ( n.z > 0.008856 ) ? pow( n.z, 1.0 / 3.0 ) : ( 7.787 * n.z ) + ( 16.0 / 116.0 );
    return vec3(( 116.0 * v.y ) - 16.0, 500.0 * ( v.x - v.y ), 200.0 * ( v.y - v.z ));
}

vec3 rgb2lab(vec3 c)
{
    vec3 lab = xyz2lab(rgb2xyz(c));
    return vec3( lab.x / 100.0, 0.5 + 0.5 * ( lab.y / 127.0 ), 0.5 + 0.5 * ( lab.z / 127.0 ));
}

void main()
{
    vec2 texelSize = 1.0f / textureSize(image, 0);
    int halfSize = kernelSize / 2;
	vec2 blurDirection = (direction == 0) ? vec2(1.0f, 0.0f) : vec2(0.0f, 1.0f);
	
	const float gaussLeft = 1.0f / sqrt(2.0f * PI * sigma);
	const float twoSigmaSquared = 2.0f * sigma * sigma;

	float sum = 0.0f;
	for(int i = -halfSize; i < halfSize+1; ++i)
	{
		vec2 uv = texCoords + texelSize * blurDirection * i;
		float g = gaussLeft * exp(-(i*i)/twoSigmaSquared);
        vec3 colorCenter = texture(image, texCoords).rgb;
        vec3 colorSample = texture(image, uv).rgb;
        float range = 1.0f - distance(rgb2lab(colorCenter), rgb2lab(colorSample)) / length(vec3(1.0f));
        float weight = g * range;
		color.rgb += colorSample * weight;
		sum += weight;
	}
	color.rgb /= sum;
    color.a = 1.0f;
}
