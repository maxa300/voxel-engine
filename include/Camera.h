#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera {
public:
  glm::vec3 position;
  glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::Vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  int width, height;
  float speed = 0.1f;
  float sensitivity = 0.1f;

};

#endif //CAMERA_H
