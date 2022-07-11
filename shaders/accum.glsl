#version 450 core

uniform sampler2D accumBuffer;
uniform uint divisor;

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    outColor = vec4(texture(accumBuffer, fragTexCoord).rgb / float(divisor), 1.0);
}