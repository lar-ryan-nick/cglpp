#version 330 core
struct Material {
	sampler2D diffuseMap0;
	sampler2D diffuseMap1;
	sampler2D diffuseMap2;
	sampler2D diffuseMap3;
	sampler2D diffuseMap4;
	sampler2D diffuseMap5;
	sampler2D diffuseMap6;
	sampler2D diffuseMap7;
	sampler2D specularMap0;
	sampler2D specularMap1;
	sampler2D specularMap2;
	sampler2D specularMap3;
	sampler2D specularMap4;
	sampler2D specularMap5;
	sampler2D specularMap6;
	sampler2D specularMap7;
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
  
in vec4 color;
in vec2 texCoord;
in vec3 normalVec;
in vec3 fragmentPosition;

uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

void main() {
	FragColor = color;
	/*
	vec4 ambient = texture(material.diffuseMap0, texCoord);
	ambient += texture(material.diffuseMap1, texCoord);
	ambient += texture(material.diffuseMap2, texCoord);
	ambient += texture(material.diffuseMap3, texCoord);
	ambient += texture(material.diffuseMap4, texCoord);
	ambient += texture(material.diffuseMap5, texCoord);
	ambient += texture(material.diffuseMap6, texCoord);
	ambient += texture(material.diffuseMap7, texCoord);
	FragColor = ambient;
	vec3 difference = light.position - fragmentPosition;
	vec3 lightDir = normalize(difference);
	float theta = dot(lightDir, normalize(-light.direction));
	float distance = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	vec3 ambient = vec3(texture(material.diffuseMap0, TexCoord));
	ambient += vec3(texture(material.diffuseMap1, TexCoord));
	ambient += vec3(texture(material.diffuseMap2, TexCoord));
	ambient += vec3(texture(material.diffuseMap3, TexCoord));
	ambient += vec3(texture(material.diffuseMap4, TexCoord));
	ambient += vec3(texture(material.diffuseMap5, TexCoord));
	ambient += vec3(texture(material.diffuseMap6, TexCoord));
	ambient += vec3(texture(material.diffuseMap7, TexCoord));
	ambient *= attenuation * light.color * light.ambientStrength;
	if (theta > light.outerCutOff) {
		float epsilon   = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		vec3 norm = normalize(normalVec);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = vec3(texture(material.diffuseMap0, TexCoord));
		diffuse += vec3(texture(material.diffuseMap1, TexCoord));
		diffuse += vec3(texture(material.diffuseMap2, TexCoord));
		diffuse += vec3(texture(material.diffuseMap3, TexCoord));
		diffuse += vec3(texture(material.diffuseMap4, TexCoord));
		diffuse += vec3(texture(material.diffuseMap5, TexCoord));
		diffuse += vec3(texture(material.diffuseMap6, TexCoord));
		diffuse += vec3(texture(material.diffuseMap7, TexCoord));
		diffuse *= intensity * attenuation * light.color * light.diffuseStrength * diff;
		vec3 viewDir = normalize(viewPos - fragmentPosition);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = vec3(texture(material.specularMap0, TexCoord));
		specular += vec3(texture(material.specularMap1, TexCoord));
		specular += vec3(texture(material.specularMap2, TexCoord));
		specular += vec3(texture(material.specularMap3, TexCoord));
		specular += vec3(texture(material.specularMap4, TexCoord));
		specular += vec3(texture(material.specularMap5, TexCoord));
		specular += vec3(texture(material.specularMap6, TexCoord));
		specular += vec3(texture(material.specularMap7, TexCoord));
		specular *= intensity * attenuation * light.color * light.specularStrength * spec;
		FragColor = vec4(ambient + diffuse + specular, 1.0);
	} else {
		FragColor = vec4(ambient, 1.0);
	}
	*/
}
