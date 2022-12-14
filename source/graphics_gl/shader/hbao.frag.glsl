#version 430

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = 1.57079632679489661923;
const float PI_4 = 0.785398163397448309616;

uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform sampler2D randomMap;

uniform float sampleRadius;

layout(std140) uniform BuildIns
{
    mat4 projMat;
    mat4 viewMat;
	mat4 normalMat;
	vec4 viewportSize;
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

vec3 reconstructViewPosition(vec2 coord)
{
	float depth = texture(depthMap, coord.xy / vec2(2048.0, 2048.0)).r;
	vec2 xy = coord.xy / vec2(1024.0, 768.0) * 2.0 - 1.0;
	vec3 Pp = vec3(xy, depth);
	vec4 Pv = inverse(projMat) * vec4(Pp, 1.0);
	Pv /= Pv.w;
	
	return Pv.xyz;
}

// image-based horizon AO, brute-force reference
float hbao(vec3 Pv, vec3 Nv, float radius)
{
    float ao = 0.0;
	float screenRadius = (projMat * vec4(radius, 0.0, 0.0, 1.0)).r * viewportSize.x; // to optimize
	
	vec4 randomFactors = texture(randomMap, gl_FragCoord.xy / vec2(16.0));
	
	for (int i = 0; i < 16; i++) {
		vec2 sampleDir = rotate(samples[i], randomFactors.r * 3.141592653);
		
		float tangentAngle = dot(normalize(Nv), normalize(vec3(sampleDir, 0.0))) - PI_2;
		float horizonAngle = tangentAngle;
		float sampleDist = 0.0;
		
		for (float j = 1.0; j <= 4.0; j += 1.0) {
			vec2 sampleCoord = gl_FragCoord.xy + sampleDir * screenRadius / j;
			vec3 samplePv = reconstructViewPosition(sampleCoord);
			float dist = length(samplePv - Pv);
			vec3 elevationVec = samplePv - Pv;
			float elevationAngle = atan(elevationVec.z / length(elevationVec.xy));

			if (elevationAngle - horizonAngle > 0.0) 
				sampleDist = dist;
			horizonAngle = max(elevationAngle, horizonAngle);			
		}
		
		float attenuation = 1.0 - clamp(0.0, 1.0,(sampleDist / radius) * (sampleDist / radius));
		ao += attenuation * (sin(horizonAngle) - sin(0.1));
	}
	ao /= 16.0;
	
	return 1.0 - ao;
}


void main(void)
{
	vec3 Pv = reconstructViewPosition(gl_FragCoord.xy);
	vec3 Nv = texture(normalMap, gl_FragCoord.xy / vec2(2048.0, 2048.0)).rgb * 2.0 - 1.0;

	colour.rgb = vec3(hbao(Pv, Nv, sampleRadius));
	
	colour.a = 1.0;
}

