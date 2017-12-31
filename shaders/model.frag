#version 150

// in vec2 Texcoords;
in vec3 Normal;
in vec3 FragPosition;
in vec3 LightDirection;

uniform vec3  ambient_color;
uniform vec3  diffuse_color;
uniform vec3  specular_color;
uniform float specular_exponent;
uniform bool  flash;

out vec4 outColor;

vec3 light_color = vec3( 1.0,  1.0,  1.0);

void main()
{
    if (flash)
    {
        outColor = vec4(0.1, 0.1, 0.1, 1.0);
        return;
    }

    vec3  norm = normalize(Normal);

    // ambient
    float ambient_strength = 0.1;
    vec3  ambient = ambient_strength * light_color * ambient_color;  // ambient strength = 0.1

    // diffuse
    float diffuse_strength = 0.8;
    vec3  diffuse = diffuse_strength * max(dot(norm, LightDirection), 0.0) * light_color * diffuse_color;

    //specular
    float specular_strength = 0.5;
    vec3  view_dir = normalize(-FragPosition);
    vec3  reflect_dir = reflect(-LightDirection, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_exponent);
    vec3  specular = specular_strength * light_color * (spec * specular_color);

    vec3  final_color = ambient + diffuse + specular;
    outColor = vec4(final_color, 1.0);
}
