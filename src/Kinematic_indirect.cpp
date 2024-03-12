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
    Shader shader("../shaders/vertexShader.vs", "../shaders/Phong_v2.vs");

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
    glm::vec3 cameraPosition    = glm::vec3(12.0f, 12.0f, 12.0f);       // Position the camera at
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

    //Joints Material
    Vec3 * joint_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3 * joint_specular_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3_set(joint_color, 1.0, 0.2, 0.2); 
    Vec3_set(joint_specular_color, 0.6, 0.1, 0.2);

    //Axis Material
    Vec3 * axis_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3 * axis_specular_color = (Vec3 *) calloc(1, sizeof(Vec3));
    Vec3_set(axis_color, 0.7, 0.7, 0.7); 
    Vec3_set(axis_specular_color, 1.0, 1.0, 1.0);

    // Allocate memory
    Buffer * buffers    = (Buffer *)    calloc(9, sizeof(Buffer) );
    Object * objects    = (Object *)    calloc(9, sizeof(Object) );
    Sphere * joints     = (Sphere *)    calloc(5, sizeof(Sphere) );
    Cylinder * axis     = (Cylinder *)  calloc(4, sizeof(Cylinder) );

    // Create the objects and their buffers
    for (unsigned char k = 0; k < 4; k++)
    {
        joints[k]       = * Sphere_init(0.30f, 0.30f, 0.30f);
        objects[2*k]    = * Sphere_generateSurface( &joints[k], 255, 255, joint_color, joint_specular_color, 0.30f, 2.0f);
        buffers[2*k]    = * Buffer_init(&objects[2*k]);

        if (k != 1)
            axis[k]         = * Cylinder_init(2.0f, 0.20f);
        else
            axis[k]         = * Cylinder_init(4.0f, 0.20f);

        objects[2*k + 1]    = * Cylinder_generateSurface( &axis[k], 1024, 0, axis_color, axis_specular_color, 0.40f, 4.0f);
        buffers[2*k + 1]    = * Buffer_init(&objects[2*k + 1]);
    }

    //END (EFFECTOR)
    joints[4]     = * Sphere_init(0.20f, 0.20f, 0.20f);
    objects[8]    = * Sphere_generateSurface( &joints[4], 255, 255, axis_color, axis_specular_color, 0.40f, 4.0f);
    buffers[8]    = * Buffer_init(&objects[8]);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    
    // Bind the shader program
    shader.use();

    //Setup Transforms
    Transform ** T_initial = (Transform **) calloc(8 , sizeof( Transform * ) );

    // ROTATION x TRANSLATION
    T_initial[0] = Transform_create(0.0f,     {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} );
    T_initial[1] = Transform_create(0.0f,     {0.0, 1.0, 0.0}, {0.0, 2.0, 0.0}, {1.0, 1.0, 1.0} );

    T_initial[2] = Transform_create(-90.0f,   {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} );
    T_initial[3] = Transform_create(0.0f,     {0.0, 1.0, 0.0}, {0.0, 4.0, 0.0}, {1.0, 1.0, 1.0} );

    T_initial[4] = Transform_create(90.0f,    {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} );
    T_initial[5] = Transform_create(0.0f,     {0.0, 1.0, 0.0}, {0.0, 2.0, 0.0}, {1.0, 1.0, 1.0} );

    T_initial[6] = Transform_create(90.0f,    {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} );
    T_initial[7] = Transform_create(0.0f,     {0.0, 1.0, 0.0}, {0.0, 2.0, 0.0}, {1.0, 1.0, 1.0} );

    IKSolver * solver = IKSolver_init(T_initial, {0.0, 2.0, 2.0}, 8);
    Transform ** t_indirect = IKSolver_solve(solver);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // Input handling (if needed)
        
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers;

        glm::mat4 M = u_model;
        shader.setMat4("u_model", M , GL_FALSE);

        float t = std::sqrt(std::min( (float) glfwGetTime() / 6.0f, 1.0f ));

        // Draw
        for (unsigned char i = 0; i < 4; i++)
        {
            Buffer_draw(&buffers[2*i]);

            //ROTATION
            M = M * Transform_getMatrix( Transform_interpolate( std::min( t , 1.0f ), T_initial[2*i], t_indirect[2*i], false ) );
            shader.setMat4("u_model", M , GL_FALSE);

            Buffer_draw(&buffers[2*i + 1]);

            //TRANSLATION
            M = M * Transform_getMatrix(T_initial[2*i + 1]);
            shader.setMat4("u_model", M , GL_FALSE);
        }

        Buffer_draw(&buffers[8]);

        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Unbind the VAO
    glBindVertexArray(0);

    // Free the allocated memory
    Texture_free(envmap);

    free(buffers);
    free(objects);
    free(joints);
    free(axis);
    free(axis_color);
    free(axis_specular_color);
    free(joint_color);
    free(joint_specular_color);
    free(solver);

    for (unsigned char i = 0; i < 8; i++)
        free(T_initial[i]);
    
    free(T_initial);

    for (unsigned char i = 0; i < 8; i++)
        free(t_indirect[i]);
    
    free(t_indirect);
    
    shader.erase();
    // Clean up and terminate GLFW
    glfwTerminate();
    return 0;
}