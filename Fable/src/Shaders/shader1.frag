#version 450

layout (location = 0) out vec4 outFragColour;
layout (location = 0) in vec3 positions;

void main() {
    outFragColour = vec4(positions.r, positions.g, positions.b, 1.0);
}