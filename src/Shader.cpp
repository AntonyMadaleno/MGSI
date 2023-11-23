#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "include/glm/gtc/type_ptr.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Read shader source code from files
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    uniforms = (Uniform *) calloc(65536, sizeof(Uniform));
    uniform_count = 0;

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // Read file contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Error: Failed to read shader files." << std::endl;
    }

    // Compile and link shaders
    compileShader(vertexCode.c_str(), fragmentCode.c_str());
    printf("shader %i created with success !\n", ID);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) {
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform1i(uloc, (int)value);
    uniforms[uniform_count].type = BOOL;
    unsigned char * v = (unsigned char *) calloc( 1, sizeof(unsigned char *));
    * v = (int)value;
    uniforms[uniform_count].value = v;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setInt(const std::string& name, int value) {
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform1i(uloc, value);
    uniforms[uniform_count].type = INT;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setFloat(const std::string& name, float value) {
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform1f(uloc, value);
    uniforms[uniform_count].type = FLOAT;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setVec2(const std::string& name, glm::vec2& value) {
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform2fv(uloc, 1, glm::value_ptr(value) );
    uniforms[uniform_count].type = VEC2;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setVec3(const std::string& name, glm::vec3& value) 
{
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform3fv(uloc, 1, glm::value_ptr(value) );
    uniforms[uniform_count].type = VEC3;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setVec3_array(const std::string& name, glm::vec3& value, const unsigned char array_size)
{
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform3fv(uloc, array_size, glm::value_ptr(value) );
    uniforms[uniform_count].type = VEC3;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setVec4(const std::string& name, glm::vec4& value) {
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniform4fv(uloc, 1, glm::value_ptr(value) );
    uniforms[uniform_count].type = VEC4;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setMat3(const std::string& name, glm::mat3& value, bool transpose)
{
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniformMatrix3fv(uloc, 1, transpose, glm::value_ptr(value) );
    uniforms[uniform_count].type = MAT3;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::setMat4(const std::string& name, glm::mat4& value, bool transpose)
{
    GLint uloc = glGetUniformLocation(ID, name.c_str());
    if (uloc == -1)
    {
        printf("Uniform %s is not located !\n", name.c_str());
        return;
    }

    glUniformMatrix4fv(uloc, 1, transpose, glm::value_ptr(value) );
    uniforms[uniform_count].type = MAT3;
    uniforms[uniform_count].value = &value;
    uniforms[uniform_count].name = name;
    uniform_count++;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
    // Vertex Shader
    unsigned int vertex, fragment;

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Error: Shader compilation error of type: " << type << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Error: Shader linking error of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}

void Shader::erase()
{
    free(uniforms);
}