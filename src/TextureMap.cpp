#include <stb/stb_image.h>
#include "TextureMap.h"

cgl::TextureMap::Type cgl::TextureMap::typeFromAssimp(aiTextureType assimpType) {
	switch (assimpType) {
		case aiTextureType_AMBIENT:
			return AMBIENT_MAP;
		case aiTextureType_DIFFUSE:
			return DIFFUSE_MAP;
		case aiTextureType_SPECULAR:
			return SPECULAR_MAP;
		case aiTextureType_EMISSIVE:
			return EMISSIVE_MAP;
		case aiTextureType_HEIGHT:
			return HEIGHT_MAP;
		case aiTextureType_NORMALS:
			return NORMALS_MAP;
		case aiTextureType_SHININESS:
			return SHININESS_MAP;
		case aiTextureType_OPACITY:
			return OPACITY_MAP;
		case aiTextureType_DISPLACEMENT:
			return DISPLACEMENT_MAP;
		case aiTextureType_LIGHTMAP:
			return LIGHT_MAP;
		case aiTextureType_REFLECTION:
			return REFLECTION_MAP;
		default:
			return NO_MAP;
	}
}

cgl::TextureMap::Operation cgl::TextureMap::operationFromAssimp(aiTextureOp assimpOp) {
	switch (assimpOp) {
		case aiTextureOp_Multiply:
			return MULT_OP;
		case aiTextureOp_Add:
			return ADD_OP;
		case aiTextureOp_Subtract:
			return SUB_OP;
		case aiTextureOp_Divide:
			return DIV_OP;
		case aiTextureOp_SmoothAdd:
			return SMOOTH_OP;
		case aiTextureOp_SignedAdd:
			return SIGNED_OP;
		default:
			return NO_OP;
	}
}

GLint cgl::TextureMap::mapModeFromAssimp(aiTextureMapMode assimpMapMode) {
	switch (assimpMapMode) {
		case aiTextureMapMode_Clamp:
			return GL_CLAMP_TO_EDGE;
		case aiTextureMapMode_Decal:
			return GL_CLAMP_TO_BORDER;
		case aiTextureMapMode_Mirror:
			return GL_MIRRORED_REPEAT;
		default:
			return GL_REPEAT;
	}
}

cgl::TextureMap cgl::TextureMap::textureMapFromAssimp(aiMaterial* material, const aiScene* scene, const std::string& directory, aiTextureType type, int index) {
	TextureMap textureMap;
	textureMap.strength = 1.0f;
	aiString str;
	aiTextureOp textureOperation(aiTextureOp_Add);
	aiTextureMapMode textureMapMode[3] = {};
	// TODO: return val check?
	material->GetTexture(type, index, &str, NULL, &textureMap.uvIndex, &textureMap.strength, &textureOperation, textureMapMode);
	const aiTexture* texture = scene->GetEmbeddedTexture(str.C_Str());
	if (texture != NULL) {
		int w;
		int h;
		unsigned char* data = reinterpret_cast<unsigned char*>(texture->pcData);
		stbi_set_flip_vertically_on_load(true);
		if (texture->mHeight == 0) {
			data = stbi_load_from_memory(data, texture->mWidth, &w, &h, NULL, 4);
		} else {
			data = stbi_load_from_memory(data, texture->mWidth * texture->mHeight, &w, &h, NULL, 4);
		}
		textureMap.texture.setTexture(data, w, h);
		stbi_image_free(data);
	} else {
		textureMap.texture.setTexture(directory + str.C_Str());
	}
	textureMap.type = typeFromAssimp(type);
	textureMap.operation = operationFromAssimp(textureOperation);
	textureMap.texture.bind();
	// tiling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mapModeFromAssimp(textureMapMode[0]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mapModeFromAssimp(textureMapMode[1]));
	return textureMap;
}

cgl::TextureMap::TextureMap() : type(NO_MAP), operation(NO_OP), strength(0.0f), uvIndex(0) {}

cgl::Texture cgl::TextureMap::getTexture() const {
	return texture;
}

cgl::TextureMap::Type cgl::TextureMap::getType() const {
	return type;
}

cgl::TextureMap::Operation cgl::TextureMap::getOperation() const {
	return operation;
}

float cgl::TextureMap::getStrength() const {
	return strength;
}

unsigned int cgl::TextureMap::getUVIndex() const {
	return uvIndex;
}
