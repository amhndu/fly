#version 330 core

in vec2 Texcoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D tex;

void main()
{
    outColor = texture(tex, Texcoords);
}
