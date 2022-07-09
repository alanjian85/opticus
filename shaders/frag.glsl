#version 450 core

uniform float aspectRatio;

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    aspectRatio; // unused variable
    outColor = vec4(fragTexCoord, 0.0, 1.0);
}