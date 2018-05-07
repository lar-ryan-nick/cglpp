#version 330 core
struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	vec3 color;
};

out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;
in vec3 normalVec;
in vec3 fragmentPosition;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directionalLight;

void main() {
	vec3 norm = normalize(normalVec);
	vec3 lightDir = normalize(-directionalLight.direction);
	vec3 ambient = directionalLight.color * directionalLight.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = directionalLight.color * directionalLight.diffuseStrength * diff * vec3(texture(material.diffuseMap, TexCoord));
	vec3 viewDir = normalize(viewPos - fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = directionalLight.color * directionalLight.specularStrength * spec * vec3(texture(material.specularMap, TexCoord));
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
