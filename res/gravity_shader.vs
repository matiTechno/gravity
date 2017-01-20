#version 330

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

uniform mat4 projection;

out vec4 outColor;

void main()
{
    gl_Position = projection * vec4(position, 0, 1);
    outColor = color;
}
