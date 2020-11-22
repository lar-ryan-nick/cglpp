#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Position.h"

namespace cgl {
	class Camera {
		private:
			glm::vec3 position;
			glm::vec3 direction;
			glm::vec3 worldUp;
		public:
			Camera(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& dir = glm::vec3(0.0f, 0.0f, 1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 getViewMatrix() const;
			glm::vec3 getPosition() const;
			glm::vec3 getDirection() const;
			void setPosition(const glm::vec3& pos);
			void setRotation(float pitch, float yaw);
			void moveForward(float amount);
			void moveRight(float amount);
	};
}

#endif
