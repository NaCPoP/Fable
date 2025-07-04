#version 450

layout(location = 0) in vec3 positions;
layout(location = 0) out vec3 out_pos;

layout(set = 0, binding = 0) uniform globalUniformObject {
    mat4 projection;
    mat4 view;
} globalUBO;

layout(push_constant) uniform PushConstants{
    mat4 model;
} pushConstants;

void main() {
    gl_Position = globalUBO.projection * globalUBO.view * pushConstants.model * vec4(positions, 1.0);
    out_pos = positions;
}