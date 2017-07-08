#version 330 core

in vec3 Position;

uniform samplerCube textureCube;

out vec4 outColor;

void main()
{
//     vec3 color = mix(vec3(0.39, 0.98, 1.0), vec3(0.067, 0.51, 0.77), clamp(abs(Position.z) * 0.4, 0.0, 1.0));
    outColor = texture(textureCube, vec3(Position.x, Position.z + 0.00, Position.y));
}
