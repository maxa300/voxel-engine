#ifndef SHADER_H
#define SHADER_H

// include glad to get the required OpenGL headers
#include "../include/glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ogl {

class Shader {
public:
  // the program ID
  unsigned int ID;
  // constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath) {
    // Retrieve the vertex/gragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      // Open Files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;

      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      // Close file handlers
      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
      std::cout << "ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    // Get Char* from std::string
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // Compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    getCompileErrors(fragment, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    getCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    getCompileErrors(ID, "PROGRAM");
  }

  void use() { glUseProgram(ID); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }

  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

private:
  void getCompileErrors(unsigned int id, std::string type) {
    int success;
    char infoLog[512];
    if (type == "PROGRAM") {
      glGetProgramiv(id, GL_LINK_STATUS, &success);
      std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    } else {
      glGetShaderiv(id, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
      }
    }
  }
};

} // namespace ogl

#endif
