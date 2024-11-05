#version 330 core
out vec4 FragColor;

in vec3 ourColor; // We set this variable in the openGL code
in vec4 position;

void main()
{
   FragColor = vec4(position);
}

