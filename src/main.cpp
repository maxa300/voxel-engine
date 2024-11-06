#include "../include/Shader.h"
#include <GL/glu.h>        // OpenGL Utility Library (GLU) functions, if needed
#include <SFML/Window.hpp> // SFML window creation and event handling
#include <SFML/System/Clock.hpp> // SFML for timing functions
#include <SFML/System/Time.hpp>  // SFML time handling
#include <cmath>                 // For math functions like std::sin
#include <iostream>              // For standard I/O (e.g., std::cerr)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void setWireframe(bool wireFrame) {
  if (wireFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main() {
  // SFML Window and Context Settings
  sf::ContextSettings settings;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.attributeFlags = sf::ContextSettings::Core;
  sf::Window window(sf::VideoMode(800, 600), "float", sf::Style::Default,
                    settings);
  window.setVerticalSyncEnabled(true);

  // Init Glad
  if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  float vertices[] = {
    // positions      // colors         // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Create Shaders
  Shader shader("../shaders/firstVertShader.vert",
                     "../shaders/firstShader.frag");

  // Instantiate sfml clock object
  sf::Clock clock;

  // Process of Generating a texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture((GL_TEXTURE_2D), texture);
  // Set the texture wrapping/filtering (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load and generate the texture
  int width, height, nrChannels;

  stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load("../textures/grass_dirt.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture((GL_TEXTURE_2D), texture2);
  // Set the texture wrapping/filtering (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char *data2 = stbi_load("../textures/happy_face.png", &width, &height, &nrChannels, 0);
  if (data2) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
    GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data2);

  // Main loop
  bool running = true;
  while (running) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        running = false;
      else if (event.type == sf::Event::Resized)
        glViewport(0, 0, event.size.width, event.size.height);
    }

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "texture2"), 1);

    // Draw the triangles
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Display the frame
    window.display();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  return 0;
}
