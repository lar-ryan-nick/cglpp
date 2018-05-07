#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "light.h"

class DirectionalLight : virtual public Light {
	glm::vec3 direction;
	public:
		DirectionalLight(const glm::vec3& dir = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), float ambientStrength = 0.3f, float diffuseStrength = 0.8f, float specularStrength = 1.0f);
		glm::vec3 getDirection() const;
		void setDirection(const glm::vec3& dir);
};

#endif
