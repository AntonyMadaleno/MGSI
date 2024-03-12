#version 330 core

#define G vec3(0.0, -9.81, 0.0)

layout(location = 0) in vec3  a_Position;    // Vertex position
layout(location = 1) in vec3  a_Normal;   	 // Vertex normal
layout(location = 2) in vec3  a_Color; 		 // Object color
layout(location = 3) in vec3  a_Specular; 	 // Object specular color
layout(location = 4) in float a_Shininess;   // Object shininess
layout(location = 5) in float a_Reflect;     // Object reflectivness

out vec3  FragPos;    // FragPos in world space
out vec3  Normal;     // Normal in world space
out vec3  Color;      // Object color
out vec3  Specular_color;
out float Shininess;
out float Reflection;

uniform mat4 u_model; 			// Model matrix
uniform mat4 u_view;  			// View matrix
uniform mat4 u_projection; 		// Projection matrix

uniform float u_time;
uniform float u_CX;
uniform float u_S;
uniform float u_air_density;
uniform vec3 u_initial_speed;

void main() {
    // Calculate the new position based on physics simulation
    vec3 v = G * u_time + u_initial_speed;
    vec3 R = 0.5 * u_CX * u_air_density * u_S * length(v) * length(v) * normalize(v);
    
    // Calculate the net force acting on the particle
    vec3 netForce = G - R;

    // Calculate the new position using the net force
    vec3 pos = a_Position + (u_initial_speed * u_time) + 0.5 * netForce * u_time * u_time;

    FragPos = vec3(u_model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(u_model))) * a_Normal;

    Color = a_Color;
    Specular_color = a_Specular;
    Shininess = a_Shininess;
    Reflection = a_Reflect;

    gl_Position = u_projection * u_view * vec4(FragPos, 1.0);
}