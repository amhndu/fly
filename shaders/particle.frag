#version 330 core

// Interpolated values from the vertex shaders
in vec2 texcoord;
in vec4 particlecolor;

// Ouput data
out vec4 color;

uniform sampler2D tex;

void main(){
	// Output color = color of the texture at the specified UV
    color = texture( tex, texcoord ) * particlecolor;
}
