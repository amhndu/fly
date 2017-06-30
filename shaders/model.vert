#version 130

in vec3 position;
// in vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// out vec3 Color;
// out vec2 Texcoords;

void main()
{
//     Color = color;
//     Texcoords = texcoords;
    gl_Position = proj * view * model * vec4(position, 1.0);
}
