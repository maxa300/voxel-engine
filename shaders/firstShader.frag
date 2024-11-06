#version 330 core
out vec4 FragColor;

in vec3 ourColor; // We set this variable in the openGL code
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;

void main()
{
   vec2 temp = vec2(-TexCoord.x, TexCoord.y);
   FragColor = mix(texture(ourTexture, TexCoord),
      texture(texture2, temp), 0.2);
}

