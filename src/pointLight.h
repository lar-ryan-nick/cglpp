#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "light.h"

class PointLight : virtual public Light {
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	public:
		PointLight(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), float ambientStrength = 0.3f, float diffuseStrength = 0.8f, float specularStrength = 1.0f, float con = 1.0f, float lin = 0.09f, float quad = 0.032f);
		glm::vec3 getPosition() const;
		float getConstant() const;
		float getLinear() const;
		float getQuadratic() const;
		void setPosition(const glm::vec3& pos);
};

#endif
