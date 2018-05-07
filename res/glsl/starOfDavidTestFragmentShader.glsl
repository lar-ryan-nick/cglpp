#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;

uniform vec3 lightPos;

void main() {
	FragColor = ourColor;
}
