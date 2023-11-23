#version 330 core
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

void main() {

    FragPos = vec3(u_model * vec4(a_Position, 1.0));
    Normal = mat3(transpose(inverse(u_model))) * a_Normal;

    Color = a_Color; // Pass the color to the fragment shader
    Specular_color = a_Specular;
    Shininess = a_Shininess;
    Reflection = a_Reflect;

    gl_Position = u_projection * u_view * vec4(FragPos, 1.0);

}
