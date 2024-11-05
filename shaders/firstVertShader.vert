#version 330 core

layout (location = 0) in vec3 aPos;   // position has attribute position 0
layout (location = 1) in vec3 aColor; // color has attribut position 1

uniform vec3 offset;

out vec3 ourColor; //output a color to the fragment shader
out vec4 position;


void   main(){
  gl_Position = vec4((aPos), 1.0);
  position = gl_Position;
  ourColor = aColor; // set ourColor to input color from the vertex data
}
