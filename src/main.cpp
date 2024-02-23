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
#include "Surface.h"

#include "Buffer.h"
#include "Image.h"
#include "Skybox.h"
#include "Texture.h"

//GLM INCLUDES
#include "include/glm/vec3.hpp" // glm::vec3
#include "include/glm/vec4.hpp" // glm::vec4
#include "include/glm/mat4x4.hpp" // glm::mat4
#include "include/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

#define V_SYNC 1

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
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
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
    Shader shader("../shaders/vertexShader.vs", "../shaders/Phong_v2.vs");

    // Use the shader program
    shader.use();

    //Set Shader uniforms

    //lights
    shader.setFloat("u_specularStrength", 0.5f);

    unsigned char light_count = 1;
    shader.setInt("u_light_count", light_count);

    glm::vec3 * lights_Colors = (glm::vec3 *) calloc(light_count , sizeof(glm::vec3));
    glm::vec3 * lights_Positions = (glm::vec3 *) calloc(light_count , sizeof(glm::vec3));

    lights_Colors[0]    = glm::vec3( 1.0f,  1.0f,  1.0f);
    lights_Positions[0] = glm::vec3( 0.0f,  5.0f,  1.0f);

    //lights_Colors[1]    = glm::vec3( 1.0f,  1.0f,  1.0f);
    //lights_Positions[1] = glm::vec3(-5.0f,  5.0f,  -5.0f);
    
    shader.setVec3_array("u_lights_Colors", lights_Colors[0], light_count);
    shader.setVec3_array("u_lights_Positions", lights_Positions[0], light_count);

    //MVP
    glm::mat4 u_model(1.0f);
    glm::mat4 u_view(1.0f);
    glm::mat4 u_projection(1.0f);

    // View matrix: Set up the camera position and orientation
    glm::vec3 cameraPosition    = glm::vec3(0.0f, 0.0f, -10.0f);     // Position the camera at
    glm::vec3 cameraTarget      = glm::vec3(0.0f, 0.0f, 0.0f);       // Look at the origin
    glm::vec3 cameraUp          = glm::vec3(0.0f, 1.0f, 0.0f);       // Set the up vector

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
    const char* fpath = "../textures/skybox/Fall_Creek.bmp";

    Texture * skybox = Texture_init(fpath);
    shader.setInt("u_environmentMap", skybox->ID);

    // Define control points for the Catmull-Rom spline
    Vec3 * control_points = (Vec3 *)calloc(11, sizeof(Vec3));

    Vec3_set(&control_points[0],  -6.0f,  0.0f,   0.0f); 
    Vec3_set(&control_points[1],  -4.0f,  0.0f,  -1.0f);
    Vec3_set(&control_points[2],  -3.0f,  1.0f,   0.0f);
    Vec3_set(&control_points[3],  -2.0f,  0.0f,   1.0f);
    Vec3_set(&control_points[4],  -1.0f, -1.0f,   0.0f);
    Vec3_set(&control_points[5],   0.0f,  0.0f,  -1.0f);
    Vec3_set(&control_points[6],   1.0f,  1.0f,   0.0f);
    Vec3_set(&control_points[7],   2.0f,  0.0f,   1.0f);
    Vec3_set(&control_points[8],   3.0f, -1.0f,   0.0f);
    Vec3_set(&control_points[9],   4.0f,  0.0f,  -1.0f);
    Vec3_set(&control_points[10],  6.0f,  0.0f,   0.0f);

    // Create a Curve3D structure to hold the result
    Curve3D * curve = Curve3D_init(control_points, 11, 8000, CATMULL_ROM);
    Curve3D_calculateTNB(curve);
    free(control_points);

    Vec3 * obj_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3 * obj_specular_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3_set(obj_color, 0.8, 0.44, 0.32); 
    Vec3_set(obj_specular_color, 0.32, 0.32, 0.8); 
    Object * obj = Curve3D_generateSurface(curve, 128, obj_color, obj_specular_color, 1.0f, 0.25f, 0.25f);

    free(obj_color);
    free(obj_specular_color);

    Buffer * buffer_0 = Buffer_init(obj);

/**

    Vec3 ** controls = (Vec3 **) calloc(3, sizeof(Vec3 *));
    
    controls[0] = (Vec3 *) calloc(3, sizeof(Vec3));
    controls[1] = (Vec3 *) calloc(3, sizeof(Vec3));
    controls[2] = (Vec3 *) calloc(3, sizeof(Vec3));

    Vec3_set(&controls[0][0], -2.0f, -1.0f, -2.0f);
    Vec3_set(&controls[0][1], -2.0f,  1.0f,  0.0f);
    Vec3_set(&controls[0][2], -2.0f, -1.0f,  2.0f);

    Vec3_set(&controls[1][0],  0.0f,  1.0f, -2.0f);
    Vec3_set(&controls[1][1],  0.0f,  3.0f,  0.0f);
    Vec3_set(&controls[1][2],  0.0f,  1.0f,  2.0f);

    Vec3_set(&controls[2][0],  2.0f, -1.0f, -2.0f);
    Vec3_set(&controls[2][1],  2.0f,  1.0f,  0.0f);
    Vec3_set(&controls[2][2],  2.0f, -1.0f,  2.0f);

    Surface3D * surface = Surface3D_init(controls, 3, 3, 255, 255, BEZIER, BEZIER);

    Vec3 * obj_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3 * obj_specular_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3_set(obj_color, 0.4, 0.2, 0.1); 
    Vec3_set(obj_specular_color, 1.0, 1.0, 1.0);

    Object * obj = Surface3D_obejctify(surface, obj_color, obj_specular_color, 1.0f, 0.1f);

    Buffer * buffer_0 = Buffer_init(obj);
**/


/**
    Vec3 * sphere_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3 * sphere_specular_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3_set(sphere_color, 0.4, 0.2, 0.1); 
    Vec3_set(sphere_specular_color, 1.0, 0.5, 0.2);

    Sphere * sphere = Sphere_init(1.0f, 1.0f, 1.0f);

    Object * sphere_object = Sphere_generateSurface(sphere, 255, 255, sphere_color, sphere_specular_color, 0.75f, 64.0f);

    Buffer * buffer_sphere = Buffer_init(sphere_object);
**/

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    
    // Bind the shader program
    shader.use();

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // Input handling (if needed)
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers

        // Set the model matrix to rotate the scene
        glm::mat4 u_model(1.0f);
        float angle = glm::radians((float)glfwGetTime() * 12.0f);
        shader.setVec3_array("u_lights_Positions", lights_Positions[0], light_count);
        cameraPosition = glm::vec3(10.0f * cosf(angle), 0.0f, 10.0f * sinf(angle));
        u_view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        shader.setMat4("u_view", u_view, GL_FALSE);
        shader.setVec3("u_viewPos", cameraPosition);

        // Draw
        Buffer_draw(buffer_0);

        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Unbind the VAO
    glBindVertexArray(0);

    Texture_free(skybox);
    
    shader.erase();
    // Clean up and terminate GLFW
    glfwTerminate();
    return 0;
}