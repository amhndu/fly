#version 330 core

in vec2 Texcoords;
in float height;

layout (location = 0) out vec4 outColor;

uniform sampler2D tex;

void main()
{
    vec3 color = mix(vec3(0.14, 1.0, 0.31), vec3(0.40, 0.72, 0.46), 0);
    outColor = texture(tex, Texcoords);
}
