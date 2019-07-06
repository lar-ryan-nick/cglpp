#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>
#include <assimp/scene.h>
#include <list>
#include "TextureMap.h"
#include "Color.h"

namespace cgl {
	class Material {
		private:
			Color ambientColor;
			Color diffuseColor;
			Color specularColor;
			std::list<TextureMap> textureMaps;
			float shininess;
			float opacity;
			static void loadMaterialTextureMaps(aiMaterial* material, aiTextureType type, const aiScene* scene, const std::string& directory, std::list<TextureMap>& textureMaps);
		public:
			static Material materialFromAssimp(aiMaterial* m, const aiScene* scene, const std::string& directory);
			Material();
			Color getAmbientColor() const;
			Color getDiffuseColor() const;
			Color getSpecularColor() const;
			std::list<TextureMap> getTextureMaps() const;
			float getShininess() const;
			float getOpacity() const;
	};
}

#endif
