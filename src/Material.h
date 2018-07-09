#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>
#include <list>
#include "Texture.h"

namespace cgl {
	class Material {
		private:
			std::list<Texture> diffuseMaps;
			std::list<Texture> specularMaps;
			float shininess;
		public:
			Material(const std::list<Texture>& diffuses, const std::list<Texture>& speculars, float shine);
			std::list<Texture> getDiffuseMaps() const;
			std::list<Texture> getSpecularMaps() const;
			float getShininess() const;
	};
}

#endif
