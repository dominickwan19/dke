#version 430

layout(location = 0) in vec4 Pv;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 colour;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outMaterial;
layout(location = 2) out vec4 outSurfaceColour;

void main(void)
{
	outNormal = vec4(normal, 1.0);
	outMaterial = vec4(0.0);
	outSurfaceColour = vec4(colour.rgb, 1.0);
}


