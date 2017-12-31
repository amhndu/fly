#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 position; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color;
layout(location = 3) in float size;

// Output data ; will be interpolated for each fragment.
out vec2 texcoord;
out vec4 particlecolor;

// Values that stay constant for the whole mesh.
uniform mat4 proj;
uniform mat4 view;
// uniform float size;

void main()
{
    vec3 vertexPosition_worldspace =
        position
        + vec3(transpose(view)[0]) * vertex.x * size
        + vec3(transpose(view)[1]) * vertex.z * size;

    // Output position of the vertex
    gl_Position = proj * view * vec4(vertexPosition_worldspace, 1.0);
    // UV of the vertex. No special space for this one.
    texcoord = vertex.xz + vec2(0.5, 0.5);
    texcoord.y = 1 - texcoord.y;
    particlecolor = color;
}
