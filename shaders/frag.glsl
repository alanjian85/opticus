#version 450 core

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    outColor = vec4(fragTexCoord, 0.0, 1.0);
}