#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cgl {
	class Light {
		private:
			glm::vec3 color;
			float ambientStrength;
			float diffuseStrength;
			float specularStrength;
		public:
			Light(const glm::vec3& col = glm::vec3(1.0, 1.0, 1.0), float ambient = 0.3f, float diffuse = 0.8f, float specular = 1.0f);
			glm::vec3 getColor() const;
			float getAmbientStrength() const;
			float getDiffuseStrength() const;
			float getSpecularStrength() const;
			void setColor(const glm::vec3& col);
			void setAmbientStrength(float ambient);
			void setDiffuseStrength(float diffuse);
			void setSpecularStrength(float specular);
	};
}

#endif
