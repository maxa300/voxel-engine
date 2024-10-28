#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h> // Core OpenGL functions
#include <GL/glext.h>
#include <GL/glu.h> // Utility functions (optional)
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <stdbool.h>
#include <string>

class Shader {
public:
  Shader(const std::string vertexShaderPath,
         const std::string fragmentShaderPath);
  ~Shader();

  int getUniformLocation(const char *uniformName);
  void use() const;
  unsigned int getID() const;

private:
  unsigned int shaderProgramID;
  std::string readShaderSource(const std::string);
};

#endif // SHADER_H
