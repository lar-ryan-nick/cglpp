#ifndef CGL_TRANSFORMABLE_H
#define CGL_TRANSFORMABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cgl {
	class Transformable {
		public:
			Transformable();
			glm::vec3 getTranslation() const;
			void translate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
			void setTranslation(const glm::vec3& t);
			float getRotation() const;
			void rotate(float radians);
			void setRotation(float radians);
			glm::vec3 getScalar() const;
			void scale(float x = 1.0f, float y = 1.0f, float z = 1.0f);
			void setScalar(const glm::vec3& s);
			glm::mat4 getTransformationMatrix() const;
			glm::mat4 getInverseTransformationMatrix() const;
		protected:
			void setCenter(const glm::vec3& c);
		private:
			glm::vec3 translation;
			glm::vec3 scalar;
			float rotation;
			glm::vec3 center;
	};
}

#endif
