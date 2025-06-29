#version 450

layout(location = 0) in vec3 positions;

void main() {
    gl_Position = vec4(positions, 1.0);
}