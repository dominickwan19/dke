#version 430

uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform sampler2D randomMap;

layout(std140) uniform BuildIns
{
    mat4 projMat;
    mat4 viewMat;
	mat4 normalMat;
};

out vec4 colour;

// Samples generated by Hammersley sequence
vec2 samples[16] = vec2[16](
	vec2(0,0),
	vec2(-0.875,0),
	vec2(-0.75,-0.5),
	vec2(-0.625,0.5),
	vec2(-0.5,-0.75),
	vec2(-0.375,0.25),
	vec2(-0.25,-0.25),
	vec2(-0.125,0.75),
	vec2(0,-0.875),
	vec2(0.125,0.125),
	vec2(0.25,-0.375),
	vec2(0.375,0.625),
	vec2(0.5,-0.625),
	vec2(0.625,0.375),
	vec2(0.75,-0.125),
	vec2(0.875,0.875)
);

vec2 rotate(vec2 v, float a) {
    float s = sin(a);
    float c = cos(a);
    mat2 m = mat2(c, s, -s, c);
    return m * v;
}

// brute-force SSAO
float ssaoBasic()
{
    float ao = 0.0;
	float depth = texture(depthMap, gl_FragCoord.xy / vec2(2048.0, 2048.0)).r;
	float weightSum = 0.0;
	
	vec4 randomFactors = texture(randomMap, gl_FragCoord.xy / vec2(16.0));
	
	// close field
	for (int i = 0; i < 16; i++)  {
		float refDepth = texture(depthMap, (gl_FragCoord.xy + rotate(samples[i], randomFactors.r * 3.141592653) * 15.0) / vec2(2048.0, 2048.0)).r;
		
		if (refDepth < depth)
			ao += 1.0 * smoothstep(-0.2, 1.0, 1.0 - length(samples[i]));
	}
	//ao /= 16.0;
	
	// far field
	for (int i = 0; i < 16; i++)  {
		float refDepth = texture(depthMap, (gl_FragCoord.xy + rotate(samples[i], randomFactors.g * 3.141592653) * 100.0) / vec2(2048.0, 2048.0)).r;
		
		if (refDepth < depth)
			ao += 1.0 * smoothstep(0.2, 0.5, 1.0 - length(samples[i]));
	}
	ao /= 32.0;
	
	return 1.0 - ao;
}

void main(void)
{
	colour.rgb = texture(normalMap, gl_FragCoord.xy / vec2(2048.0, 2048.0)).rgb;
	
	colour.rgb = vec3(ssaoBasic());
	colour.a = 1.0;
}
