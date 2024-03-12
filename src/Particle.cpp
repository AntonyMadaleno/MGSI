/**
g++ -o ../bin/main.exe main.cpp Shader.cpp Curve.c Vec.c Quaternion.c Object.c Matrix.c Image.c Buffer.cpp Skybox.cpp Texture.cpp -lglfw3 -lglew32 -lgdi32 -lopengl32 -lpthread
**/
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <unistd.h>

#include "Shader.hpp"

#include "Curve.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Surface.h"

#include "Buffer.h"
#include "Image.h"
#include "Skybox.h"
#include "Texture.h"
#include "Transform.h"

//GLM INCLUDES
#include "include/glm/vec3.hpp" // glm::vec3
#include "include/glm/vec4.hpp" // glm::vec4
#include "include/glm/mat4x4.hpp" // glm::mat4
#include "include/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "include/glm/gtc/random.hpp"
#include "include/glm/gtx/rotate_vector.hpp"

#define V_SYNC 1
#define N_PARTICLE 10000

// GLFW callback function for handling window resize
void framebuffer_size_callback(GLFWwindow * window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

int main() {

    // Initialize GLFW
    if ( !glfwInit() ) 
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enable V-Sync (1) or disable it (0)
    glfwSwapInterval(V_SYNC); // Enable V-Sync (limits frame rate to the refresh rate)

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(1600, 900, "App Window", NULL, NULL);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the OpenGL viewport size and callback function
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Print the OpenGL version
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    // Create a Shader object
    Shader shader("../shaders/particle_vertex.vs", "../shaders/particle_fragment.vs");

    // Use the shader program
    shader.use();

    //Set Shader uniforms

    //lights
    shader.setFloat("u_specularStrength", 1.0f);

    unsigned char light_count = 1;
    shader.setInt("u_light_count", light_count);

    glm::vec3 * lights_Colors = (glm::vec3 *) calloc(light_count , sizeof(glm::vec3));
    glm::vec3 * lights_Positions = (glm::vec3 *) calloc(light_count , sizeof(glm::vec3));

    lights_Colors[0]    = glm::vec3( 1.0f,  1.0f,  1.0f);
    lights_Positions[0] = glm::vec3( 0.0f,  50.0f,  0.0f);

    //lights_Colors[1]    = glm::vec3( 1.0f,  1.0f,  1.0f);
    //lights_Positions[1] = glm::vec3(-5.0f,  5.0f,  -5.0f);
    
    shader.setVec3_array("u_lights_Colors", lights_Colors[0], light_count);
    shader.setVec3_array("u_lights_Positions", lights_Positions[0], light_count);

    //MVP
    glm::mat4 u_model(1.0f);
    glm::mat4 u_view(1.0f);
    glm::mat4 u_projection(1.0f);

    // View matrix: Set up the camera position and orientation
    glm::vec3 cameraPosition    = glm::vec3(0.0f, 2.0f, 20.0f);         // Position the camera at
    glm::vec3 cameraTarget      = glm::vec3(0.0f, 0.0f, 0.0f);          // Look at the origin
    glm::vec3 cameraUp          = glm::vec3(0.0f, 1.0f, 0.0f);          // Set the up vector

    u_view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    // Projection matrix: Set up the perspective projection
    float aspectRatio   = 16.f/9.f;                                 // Adjust as needed
    float fieldOfView   = glm::radians(45.0f);                      // Field of view in radians
    float nearClip      = 0.1f;                                     // Near clipping plane
    float farClip       = 100.0f;                                   // Far clipping plane

    u_projection = glm::perspective(fieldOfView, aspectRatio, nearClip, farClip);

    shader.setMat4("u_model", u_model, GL_FALSE);
    shader.setMat4("u_view", u_view, GL_FALSE);
    shader.setMat4("u_projection", u_projection, GL_FALSE);

    //view
    shader.setVec3("u_viewPos", cameraPosition);

    //LOAD SKYBOX
    const char * fpath = "../textures/skybox/Fall_Creek.bmp";

    Texture * envmap = Texture_init(fpath);
    shader.setInt("u_environmentMap", envmap->ID);

    //Define materials

    //Particle Material
    Vec3 * particle_color           = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3 * particle_specular_color  = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3_set(particle_color, 0.2, 0.2, 1.0); 
    Vec3_set(particle_specular_color, 0.1, 0.2, 0.6);

    //Base 3D model for the particle
    float radius = 0.02f;

    Sphere * particle           = Sphere_init(radius, radius, radius);
    Object * particle_object    = Sphere_generateSurface( particle, 8, 8, particle_color, particle_specular_color, 0.30f, 2.0f);
    Buffer * particle_buffer    = Buffer_init(particle_object);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    
    // Bind the shader program
    shader.use();

    // Setup the parameters for the particles uniforms
    shader.setFloat("u_CX"  , 0.30f);
    shader.setFloat("u_S"   , radius * radius * 2 * M_PI);
    shader.setFloat("u_air_density", 1.0f);

    glm::vec3 * u_speed_init = (glm::vec3 *) calloc( N_PARTICLE, sizeof(glm::vec3) ); 

    std::srand(static_cast<unsigned int>(std::time(0)));

    for (unsigned int k = 0; k < N_PARTICLE; k++)
    {

        float alpha = 2.0f * M_PI * static_cast<float>(rand()) / RAND_MAX;
        float theta = (M_PI / 20.0f) * static_cast<float>(rand()) / RAND_MAX;
        float v = cosf(theta) * cosf(theta) * static_cast<float>(rand()) / RAND_MAX * 5.0f + 25.0f;

        // Set the main axis along the y-axis
        glm::vec3 mainAxis = glm::vec3(cosf(alpha) * sinf(theta), cosf(theta), sinf(alpha) * sinf(theta));

        // Normalize the initial speed vector
        u_speed_init[k] = glm::normalize(mainAxis) * 10.0f;
    }

    float t = 0.0f;

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // Input handling (if needed)
        
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers;

        glm::mat4 M = u_model;
        shader.setMat4("u_model", M , GL_FALSE);

        t = (float) glfwGetTime() / 6.0f;

        shader.setFloat("u_time", t);

        for (unsigned int k = 0; k < N_PARTICLE; k++)
        {
            shader.setVec3("u_initial_speed", u_speed_init[k]);
            Buffer_draw(particle_buffer);
        }

        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Unbind the VAO
    glBindVertexArray(0);

    // Free the allocated memory
    Texture_free(envmap);
    
    shader.erase();
    // Clean up and terminate GLFW
    glfwTerminate();
    return 0;
}