#version 330 core
out vec4 FragColor;

in vec3 ourColor; // We set this variable in the openGL code

void main()
{
   FragColor = vec4(ourColor, 1.0);
}

