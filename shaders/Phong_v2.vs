#version 330 core

#define PI 3.1415926535897932384626433832795

out vec4 FragColor;

in vec3  FragPos;     // FragPos in world space
in vec3  Normal;      // Normal in world space
in vec3  Color;       // Color of the object
in vec3  Specular_color;
in float Shininess;
in float Reflection;

uniform vec3 u_lights_Positions[256]; // Light position
uniform vec3 u_lights_Colors[256];    // Light color
uniform int  u_light_count;           // Number of light sources, max 256
uniform vec3 u_viewPos;               // View position

uniform float u_specularStrength;

uniform sampler2D u_environmentMap; // The environment map sampler uniform

vec2 NormalToSphericalUV(vec3 normal) {

    // Map the spherical coordinates to the (u, v) texture space
    float u = (atan(normal.z, normal.x) + PI) / (2.0 * PI); 
    float v = acos(normal.y) / PI;

    return vec2(v, u);
}

void main() {

    vec3 N = normalize(Normal);
    bool isBackFace = !gl_FrontFacing;

    N = isBackFace ? -N : N;

    vec3 viewDir = normalize(u_viewPos - FragPos);

    float dotN = dot(N, viewDir);

    vec2 e_dir = NormalToSphericalUV( reflect(viewDir, N) );



    // Use the normal to look up the color from the environment map
    vec3 envMapColor = texture( u_environmentMap, e_dir ).rgb;

    // Ambient lighting
    vec3 result = envMapColor * Reflection;

    for (int i = 0; i < u_light_count; i++) {
        // Diffuse lighting
        vec3 lightDir = normalize(u_lights_Positions[i] - FragPos);
        float diff = max(dot(N, lightDir), 0.0);
        result += diff * u_lights_Colors[i] * Color;

        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, N);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
        result += u_specularStrength * spec * u_lights_Colors[i] * Specular_color;
    }

    FragColor = vec4(result, 1.0);
}
