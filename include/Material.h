#ifndef CGL_MATERIAL_H
#define CGL_MATERIAL_H

#include <glad/glad.h>
#include <assimp/scene.h>
#include <list>
#include "TextureMap.h"
#include "Color.h"

namespace cgl {
	class Material {
		public:
			static Material materialFromAssimp(aiMaterial* m, const aiScene* scene, const std::string& directory);
			Material();
			Color getAmbientColor() const;
			void setAmbientColor(const Color& color);
			Color getDiffuseColor() const;
			void setDiffuseColor(const Color& color);
			Color getSpecularColor() const;
			void setSpecularColor(const Color& color);
			std::list<TextureMap> getTextureMaps() const;
			float getShininess() const;
			void setShininess(float s);
			float getOpacity() const;
			void getOpacity(float o);
		private:
			Color ambientColor;
			Color diffuseColor;
			Color specularColor;
			std::list<TextureMap> textureMaps;
			float shininess;
			float opacity;
			static void loadMaterialTextureMaps(aiMaterial* material, aiTextureType type, const aiScene* scene, const std::string& directory, std::list<TextureMap>& textureMaps);
	};
}

#endif
