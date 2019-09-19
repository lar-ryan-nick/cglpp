#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <glad/glad.h>
#include <assimp/scene.h>
#include "Texture.h"

namespace cgl {
	class TextureMap {
		public:
			enum Operation {
				NO_OP = -1,
				MULT_OP,
				ADD_OP,
				SUB_OP,
				DIV_OP,
				SMOOTH_OP,
				SIGNED_OP
			};
			enum Type {
				NO_MAP = -1,
				AMBIENT_MAP,
				DIFFUSE_MAP,
				SPECULAR_MAP,
				EMISSIVE_MAP,
				HEIGHT_MAP,
				NORMALS_MAP,
				SHININESS_MAP,
				OPACITY_MAP,
				DISPLACEMENT_MAP,
				LIGHT_MAP,
				REFLECTION_MAP
			};
		private:
			Texture texture;
			Type type;
			Operation operation;
			float strength;
			unsigned int uvIndex;
			static Type typeFromAssimp(aiTextureType assimpType);
			static Operation operationFromAssimp(aiTextureOp assimpOp);
			static GLint mapModeFromAssimp(aiTextureMapMode assimpMapMode);
		public:
			static TextureMap textureMapFromAssimp(aiMaterial* material, const aiScene* scene, const std::string& directory, aiTextureType type, int index);
			TextureMap();
			Texture getTexture() const;
			Type getType() const;
			Operation getOperation() const;
			float getStrength() const;
			unsigned int getUVIndex() const;
	};
}

#endif
