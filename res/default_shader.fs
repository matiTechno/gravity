#version 330

in vec2 texCoord;

out vec4 color;

uniform vec4 spriteColor;

uniform sampler2D sampl;

void main()
{
    color = texture(sampl, texCoord) * spriteColor;
}
