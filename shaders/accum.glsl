#version 450 core

uniform sampler2DMS accumBuffer;
uniform int samples;
uniform int divisor;

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    ivec2 texSize = textureSize(accumBuffer);
    ivec2 texCoord = ivec2(fragTexCoord * texSize);

    vec4 color = vec4(0.0);
    for (int i = 0; i < samples; ++i) {
        color += texelFetch(accumBuffer, texCoord, i);
    }
    color /= float(samples);

    outColor = vec4(color.rgb / float(divisor), 1.0);
}