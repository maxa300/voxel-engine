#include "../include/Shader.h"
#include <GL/glu.h>        // OpenGL Utility Library (GLU) functions, if needed
#include <SFML/Window.hpp> // SFML window creation and event handling
#include <SFML/System/Clock.hpp> // SFML for timing functions
#include <SFML/System/Time.hpp>  // SFML time handling
#include <cmath>                 // For math functions like std::sin
#include <iostream>              // For standard I/O (e.g., std::cerr)

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

  // vertices for 2 triangles to make rectangle
  float vertices[] = {
      // positions       // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  // stores VBO states and data while bound
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // configure VBO that will be stored in VAO
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbind VAO (which will be rebound for later use)
  glBindVertexArray(0);

  // // Set up Element Buffer Object
  // unsigned int EBO;
  // glGenBuffers(1, &EBO);
  //
  // // Set up vertex buffer and vertex array objects
  // unsigned int VBO, VAO;
  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  //
  // glBindVertexArray(VAO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //
  // // copy our index array in a element buffer for OpenGL to use
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void
  // *)0); glEnableVertexAttribArray(0);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
  // glBindVertexArray(0);             // Unbind VAO

  // Create Shaders
  ogl::Shader shader("../shaders/firstVertShader.vert",
                     "../shaders/firstShader.frag");

  // Instantiate sfml clock object
  sf::Clock clock;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // offset triangle
    int hOffsetLocation = glGetUniformLocation(shader.ID, "offset");
    shader.use();
    glUniform3f(hOffsetLocation, 0.5f, 0.0f, 0.0f);

    // Draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Display the frame
    window.display();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  return 0;
}
