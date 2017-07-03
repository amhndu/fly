#version 330 core

in vec2 Texcoords;
in float height;

layout (location = 0) out vec4 outColor;

uniform sampler2D tex;

void main()
{
    vec3 color = mix(vec3(0, 135.0 / 255.0, 49.0 / 255.0), vec3(0, 168.0 / 255, 62.0 / 255.0), height);
    outColor = vec4(color, 1.0) * (texture(tex, Texcoords) / 3 + vec4(0.6, 0.6, 0.6, 0.6));
}
