#version 330 core

in vec3  Normal;
in vec3  LightDirection;
in vec3  FragPosition;
in float Height;
in float Color;

layout (location = 0) out vec4 outColor;

uniform sampler2D tex;

vec3 light_color = vec3( 1.0,  1.0,  1.0);

void main()
{
    vec3 color = vec3(texture(tex, vec2(1.0 - (Height / 1.5), Color)));

    vec3 norm = normalize(Normal);

    // ambient
    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * light_color * color;  // ambient strength = 0.1

    // diffuse
    float diffuse_strength = 0.8;
    vec3 diffuse = diffuse_strength * max(dot(norm, LightDirection), 0.0) * light_color * color;

    float alpha = 1.0;
    float distance = length(FragPosition);
    const float fog_begin = 7.0;
    if (distance >= fog_begin)
        alpha = exp((fog_begin - distance) / 8.0);

    vec3 perspective_color = vec3(151 / 255.0, 208 / 255.0, 212 / 255.0);

    vec3 final_color = mix(perspective_color, ambient + diffuse, alpha);
    outColor = vec4(final_color, 1.0);
}
