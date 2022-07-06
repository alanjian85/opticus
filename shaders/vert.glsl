#version 450 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main() {
    gl_Position = inPos;
    fragTexCoord = inTexCoord;
}