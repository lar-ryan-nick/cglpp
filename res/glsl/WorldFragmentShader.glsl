#version 400 core
struct Material {
	sampler2D diffuseMaps[8];
	uint numDiffuse;
	sampler2D specularMaps[8];
	uint numSpecular;
	float shininess;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;
in vec3 normalVec;
in vec3 fragmentPosition;

uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

void main() {
	vec3 difference = light.position - fragmentPosition;
	vec3 lightDir = normalize(difference);
	float theta = dot(lightDir, normalize(-light.direction));
	float distance = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	vec3 ambient;
	for (uint i = 0; i < material.numDiffuse; i++) {
		ambient += attenuation * light.color * light.ambientStrength * vec3(texture(material.diffuseMaps[i], TexCoord));
	}
	if (theta > light.outerCutOff) {
		float epsilon   = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		vec3 norm = normalize(normalVec);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse;
		for (uint i = 0; i < material.numDiffuse; i++) {
			diffuse += intensity * attenuation * light.color * light.diffuseStrength * diff * vec3(texture(material.diffuseMaps[i], TexCoord));
		}
		vec3 viewDir = normalize(viewPos - fragmentPosition);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular;
		for (uint i = 0; i < material.numSpecular; i++) {
			specular += intensity * attenuation * light.color * light.specularStrength * spec * vec3(texture(material.specularMaps[i], TexCoord));
		}
		FragColor = vec4(ambient + diffuse + specular, 1.0);
	} else {
		FragColor = vec4(ambient, 1.0);
	}
}
