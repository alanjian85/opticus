#version 450 core

uniform sampler2D accumBuffer;
uniform int samples;

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    outColor =
        vec4(texture(accumBuffer, fragTexCoord).xyz / float(samples), 1.0);
}