#version 330 core
out vec4 FragColor;

in vec3  FragPos;    // FragPos in world space
in vec3  Normal;     // Normal in world space
in vec3  Color;		// Color of object
in vec3  Specular_color;
in float Shininess;

uniform vec3 u_lights_Positions[256]; // Light position
uniform vec3 u_lights_Colors[256]; // Light color
uniform int  u_light_count; //number of light sources max 256
uniform vec3 u_viewPos;  // View position

uniform float u_ambientStrength;
uniform float u_specularStrength;

void main() {

    // Ambient lighting
    vec3 result = u_ambientStrength * Color;
    vec3 viewDir = normalize(u_viewPos - FragPos);

    for (int i = 0; i < u_light_count; i++)
    {
        // Diffuse lighting
        vec3 lightDir = normalize(u_lights_Positions[i] - FragPos);
        float diff = max(dot(Normal, lightDir), 0.0);
        result += diff * u_lights_Colors[i] * Color;

        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
        result += u_specularStrength * spec * u_lights_Colors[i];
    }
    
    FragColor = vec4(result, 1.0);
}
