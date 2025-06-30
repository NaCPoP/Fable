#version 450

layout(location = 0) in vec3 positions;

layout(set = 0, binding = 0) uniform globalUniformObject {
    mat4 projection;
    mat4 view;
} globalUBO;

void main() {
    gl_Position = globalUBO.projection * globalUBO.view * vec4(positions, 1.0);
}