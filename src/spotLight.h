#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "pointLight.h"
#include "directionalLight.h"

namespace cgl {
	class SpotLight : public DirectionalLight, public PointLight {
		private:
			float cutOff;
			float outerCutOff;
		public:
			SpotLight(const glm::vec3& dir = glm::vec3(0.0f, 0.0f, 1.0f), const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), float ambientStrength = 0.3f, float diffuseStrength = 0.8f, float specularStrength = 1.0f, float cut = glm::cos(glm::radians(12.5f)), float outerCut = glm::cos(glm::radians(17.5f)), float con = 1.0f, float lin = 0.09f, float quad = 0.032f);
			float getCutOff() const;
			void setCutOff(float cut);
			float getOuterCutOff() const;
			void setOuterCutOff(float outerCut);
	};
}

#endif
