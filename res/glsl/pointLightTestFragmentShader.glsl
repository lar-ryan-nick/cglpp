#version 330 core
struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct PointLight {
	vec3 position;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;
in vec3 normalVec;
in vec3 fragmentPosition;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight light;

void main() {
	vec3 difference = light.position - fragmentPosition;
	float distance = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	vec3 norm = normalize(normalVec);
	vec3 lightDir = normalize(difference);
	vec3 ambient = attenuation * light.color * light.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = attenuation * light.color * light.diffuseStrength * diff * vec3(texture(material.diffuseMap, TexCoord));
	vec3 viewDir = normalize(viewPos - fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = attenuation * light.color * light.specularStrength * spec * vec3(texture(material.specularMap, TexCoord));
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
