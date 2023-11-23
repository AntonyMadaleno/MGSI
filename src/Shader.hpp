/**
 * @file Shader.hpp
 * @brief Header for class Shader
 * @author Antony Madaleno
 * @version 1.0
 * @date 14-09-2023
 *
 * Header pour la classe Shader
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Vec.h"

//GLM INCLUDES
#include "include/glm/vec3.hpp" // glm::vec3
#include "include/glm/vec4.hpp" // glm::vec4
#include "include/glm/mat4x4.hpp" // glm::mat4
#include "include/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

enum uniform_type
{
    BOOL,
    INT,
    UINT,
    FLOAT,
    DOUBLE,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4,
};

/**
 * @struct Uniform
 * @brief Uniform contain data related to a given uniform
 */
typedef struct Uniform
{
    std::string name;
    enum uniform_type type;
    void * value;
} Uniform;

/**
 * @class Shader
 * @brief A class for managing GLSL shaders.
 */
class Shader {
public:
    unsigned int ID; ///< Shader program ID
    Uniform * uniforms;
    unsigned char uniform_count;

    /**
     * @brief Constructor that loads and compiles vertex and fragment shaders.
     * @param vertexPath The file path to the vertex shader.
     * @param fragmentPath The file path to the fragment shader.
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Use the shader program.
     */
    void use();

    /**
     * @brief Set a boolean uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The boolean value to set.
     */
    void setBool(const std::string& name, bool value);

    /**
     * @brief Set an integer uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The integer value to set.
     */
    void setInt(const std::string& name, int value);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The float value to set.
     */
    void setFloat(const std::string& name, float value);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The vec2 value to set.
     */
    void setVec2(const std::string& name, glm::vec2& value);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The vec3 value to set.
     */
    void setVec3(const std::string& name, glm::vec3& value);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The vec3 value to set.
     * @param array_size
     */
    void setVec3_array(const std::string& name, glm::vec3& value, const unsigned char array_size);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The vec4 value to set.
     */
    void setVec4(const std::string& name, glm::vec4& value);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The Mat3 pointer to set.
     */
    void setMat3(const std::string& name, glm::mat3& value, bool transpose);

    /**
     * @brief Set a float uniform in the shader program.
     * @param name The name of the uniform.
     * @param value The Mat4 pointer to set.
     */
    void setMat4(const std::string& name, glm::mat4& value, bool transpose);

    void erase();

private:

    /**
     * @brief Compile and link shaders.
     * @param vertexCode The source code of the vertex shader.
     * @param fragmentCode The source code of the fragment shader.
     */
    void compileShader(const char* vertexCode, const char* fragmentCode);

    /**
     * @brief Check for shader compilation/linking errors.
     * @param shader The shader ID.
     * @param type The type of shader (VERTEX, FRAGMENT, or PROGRAM).
     */
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif