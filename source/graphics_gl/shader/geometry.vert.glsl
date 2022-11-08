#version 430

layout(std140) uniform BuildIns
{
    mat4 projMat;
    mat4 viewMat;
	mat4 normalMat;
};

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Colour;

layout(location = 0) out vec4 Pv;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 colour;

void main(void)
{
    gl_Position = projMat * viewMat * vec4(in_Position, 1.0);

    Pv = viewMat * vec4(in_Position.xyz, 1.0);
    Pv /= Pv.w;

    colour = in_Colour;
	
	vec4 n = normalMat * vec4(in_Normal, 1.0);
	normal = normalize(n.xyz / n.w);
}
