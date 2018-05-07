#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;
in vec3 normalVec;
in vec3 fragmentPosition;

uniform vec3 viewPos;
uniform sampler2D sampler;
uniform vec3 lightPos;

void main() {
	vec3 difference = lightPos - fragmentPosition;
	vec3 lightDir = normalize(difference);
	vec3 norm = normalize(normalVec);
	float diff = max(dot(norm, lightDir), 0.3);
	vec3 diffuse = diff * vec3(texture(sampler, TexCoord));
	FragColor = vec4(diffuse, 1.0);
}
