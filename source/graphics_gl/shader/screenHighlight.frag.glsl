#version 430

uniform sampler2D inputTexture;
uniform sampler2D highlightMask;

out vec4 color;

void main(void)
{
    // Pixel is marked as opaque when:
    // 1. any one of 8 surrounding pixels is marked as highlighted, and
    // 2. current pixel is not marked as highlighted (we want to show the entity's original colour).
    // Otherwise the pixel is transparent.
    vec4 baseColor = vec4(0.0);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec4 c = texture(highlightMask, (gl_FragCoord.xy + vec2(x, y)) / 2048.0);
            if (c.a > 0.0) {
                baseColor = c;
                break;
            }
        }
        if (baseColor.a > 0.0)
            break;
    }

    color = baseColor;
}
