#version 330 core
// min number of supported sampler objects per fragment shader is 16
const int MAX_TEXTURE_MAPS = 16;

const int AMBIENT_MAP = 0;
const int DIFFUSE_MAP = 1;
const int SPECULAR_MAP = 2;
const int EMISSIVE_MAP = 3;
const int HEIGHT_MAP = 4;
const int NORMALS_MAP = 5;
const int SHININESS_MAP = 6;
const int OPACITY_MAP = 7;
const int DISPLACEMENT_MAP = 8;
const int LIGHT_MAP = 9;
const int REFLECTION_MAP = 10;

const int MULT_OP = 0;
const int ADD_OP = 1;
const int SUB_OP = 2;
const int DIV_OP = 3;
const int SMOOTH_OP = 4;
const int SIGNED_OP = 5;

struct TextureMap {
	sampler2D texture;
	int type;
	float strength;
	int operation;
	uint uvIndex;
};

struct Material {
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	TextureMap textureMaps[MAX_TEXTURE_MAPS];
	float shininess;
	float opacity;
};

struct DirectionalLight {
	vec3 direction;
	vec3 color;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

out vec4 FragColor;

in vec2 texCoord;
in vec3 normalVec;
in vec3 fragmentPosition;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 viewDirection;
int shininessIndex = -1;

// TODO: remove structs from parameters
void calculateTextureMap(int tmIndex, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void applyTextureMap(int tmIndex, inout vec3 base);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 ambient, vec3 diffuse, vec3 specular);
vec3 calculatePointLight(PointLight light, vec3 ambient, vec3 diffuse, vec3 specular);
vec3 calculateSpotLight(SpotLight light, vec3 ambient, vec3 diffuse, vec3 specular);

void main() {
	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 ambient = material.ambientColor;
	vec3 diffuse = material.diffuseColor;
	vec3 specular = material.specularColor;
	vec3 result = calculateDirectionalLight(directionalLight, ambient, diffuse, specular);
	//result += calculateSpotLight(spotLight, ambient, diffuse, specular);
	FragColor = vec4(result, material.opacity);
}

void calculateTextureMap(int tmIndex, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
	switch (material.textureMaps[tmIndex].type) {
		case AMBIENT_MAP:
			applyTextureMap(tmIndex, ambient);
			break;
		case DIFFUSE_MAP:
			applyTextureMap(tmIndex, diffuse);
			break;
		case SPECULAR_MAP:
			applyTextureMap(tmIndex, specular);
			break;
		default:
			break;
	}
}

void applyTextureMap(int tmIndex, inout vec3 base) {
	vec3 mapContrib = material.textureMaps[tmIndex].strength * texture(material.textureMaps[tmIndex].texture, texCoord).xyz;
	switch (material.textureMaps[tmIndex].operation) {
		case MULT_OP:
			base *= mapContrib;
			break;
		case ADD_OP:
			base += mapContrib;
			break;
		case SUB_OP:
			base -= mapContrib;
			break;
		case DIV_OP:
			base /= mapContrib;
			break;
		case SMOOTH_OP:
			base = (base + mapContrib) - (base * mapContrib);
			break;
		case SIGNED_OP:
			base += mapContrib - .5;
			break;
		default:
			break;
	}
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 ambient, vec3 diffuse, vec3 specular) {
	vec3 lightDirection = normalize(-light.direction);
	float diff = max(dot(normalVec, lightDirection), 0.0f);
	vec3 reflectDirection = reflect(-lightDirection, normalVec);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
	return light.color * (light.ambientStrength * ambient + light.diffuseStrength * diff * diffuse + light.specularStrength * spec * specular);
}

vec3 calculatePointLight(PointLight light, vec3 ambient, vec3 diffuse, vec3 specular) {
	vec3 difference = light.position - fragmentPosition;
	vec3 lightDir = normalize(difference);
	float diff = max(dot(normalVec, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normalVec);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
	float distance = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation * light.color * light.ambientStrength;
	diffuse *= attenuation * light.color * light.diffuseStrength * diff;
	specular *= attenuation * light.color * light.specularStrength * spec;
	return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 ambient, vec3 diffuse, vec3 specular) {
	vec3 difference = light.position - fragmentPosition;
	vec3 lightDir = normalize(difference);
	float theta = dot(lightDir, normalize(-light.direction));
	float distance = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation * light.color * light.ambientStrength;
	if (theta > light.outerCutOff) {
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		float diff = max(dot(normalVec, lightDir), 0.0);
		diffuse *= intensity * attenuation * light.color * light.diffuseStrength * diff;
		vec3 reflectDir = reflect(-lightDir, normalVec);
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
		specular *= intensity * attenuation * light.color * light.specularStrength * spec;
		return ambient + diffuse + specular;
	}
	return ambient;
}
