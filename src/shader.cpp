#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

Shader::Shader(const std::string vertexShaderPath,
               const std::string fragmentShaderPath) {
  // Get source for shaders
  std::string vertexSourceTemp = readShaderSource(vertexShaderPath);
  std::string fragmentSourceTemp = readShaderSource(fragmentShaderPath);
  const char *vertexShaderSource = vertexSourceTemp.c_str();
  const char *fragmentShaderSource = fragmentSourceTemp.c_str();

  // Shader Compilation
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check vertex shader compilation
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Fragment shader compilation
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check fragment shader compilation
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Link shaders
  shaderProgramID = glCreateProgram();
  glAttachShader(shaderProgramID, vertexShader);
  glAttachShader(shaderProgramID, fragmentShader);
  glLinkProgram(shaderProgramID);

  // Check shader program linking
  glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Delete unused shader now that they are linked in program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader() { glDeleteShader(shaderProgramID); }

void Shader::use() const { glUseProgram(shaderProgramID); }

unsigned int Shader::getID() const { return shaderProgramID; }

std::string Shader::readShaderSource(const std::string filePath) {
  std::ifstream shaderFile(filePath);
  if (!shaderFile.is_open()) {
    std::cerr << "Failed ot open file: " << filePath << std::endl;
    return "";
  }
  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  shaderFile.close();
  return shaderStream.str().c_str();
}

int Shader::getUniformLocation(const char *uniformName) {
  return glGetUniformLocation(shaderProgramID, uniformName);
}
