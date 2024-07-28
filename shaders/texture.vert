#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;
layout(location = 0) out vec2 vTexcoord;

layout(std140, binding = 0) uniform buf {
    mat4 matrix;
} ubuf;

void main()
{
    vTexcoord = texcoord;
    gl_Position = ubuf.matrix * position;
}
