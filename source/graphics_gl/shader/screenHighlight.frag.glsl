#version 430

uniform sampler2D inputTexture;
uniform sampler2D highlightMask;

out vec4 color;

void main(void)
{
    color = vec4(1, 1, 0, 1);
}
