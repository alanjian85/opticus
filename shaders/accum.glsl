#version 450 core

uniform sampler2DMS accumBuffer;
uniform int samples;
uniform int aaLevel;

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    ivec2 texSize = textureSize(accumBuffer);
    ivec2 texCoord = ivec2(fragTexCoord * texSize);

    vec4 color = vec4(0.0);
    for (int i = 0; i < aaLevel; ++i) {
        color += texelFetch(accumBuffer, texCoord, i);
    }
    color /= float(aaLevel);

    outColor = vec4(color.rgb / float(samples), 1.0);
}