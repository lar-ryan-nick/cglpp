#version 330 core
const int MAX_TEXTURE_MAPS = 8;

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
	float reflectivity;
};

uniform Material material;
//int shininessIndex = -1;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
//layout (location = 2) out vec3 gTangent;
layout (location = 2) out vec3 ambient;
layout (location = 3) out vec3 diffuse;
layout (location = 4) out vec4 specularReflect;

in vec2 texCoord;
in vec3 normalVec;
in vec3 tangentVec;
in vec3 fragmentPosition;

vec3 specular;

void calculateTextureMap(TextureMap tm);
void applyTextureMap(TextureMap tm, inout vec3 base);

void main() {
	// store the fragment position vector in the first gbuffer texture
	gPosition = vec4(fragmentPosition, 1.0f);
	// also store the per-fragment normals into the gbuffer
	gNormal = normalVec;
	// and the texture coordinate
	//gTangent = tangentVec;

	ambient = material.ambientColor;
	diffuse = material.diffuseColor;
	specular = material.specularColor;
	calculateTextureMap(material.textureMaps[0]);
	calculateTextureMap(material.textureMaps[1]);
	calculateTextureMap(material.textureMaps[2]);
	calculateTextureMap(material.textureMaps[3]);
	calculateTextureMap(material.textureMaps[4]);
	calculateTextureMap(material.textureMaps[5]);
	calculateTextureMap(material.textureMaps[6]);
	calculateTextureMap(material.textureMaps[7]);
	specularReflect = vec4(specular, material.reflectivity);
}

void calculateTextureMap(TextureMap tm) {
	switch (tm.type) {
		case DIFFUSE_MAP:
			applyTextureMap(tm, diffuse);
		case AMBIENT_MAP:
			applyTextureMap(tm, ambient);
			break;
		case SPECULAR_MAP:
			applyTextureMap(tm, specular);
			break;
		default:
			break;
	}
}

void applyTextureMap(TextureMap tm, inout vec3 base) {
	vec3 mapContrib = tm.strength * texture(tm.texture, texCoord).xyz;
	switch (tm.operation) {
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
