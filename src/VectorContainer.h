#ifndef VECTORCONTAINER_H
#define VECTORCONTAINER_H

#include <functional>
#include <iostream>
#include <glm/glm.hpp>

namespace cgl {
	class VectorContainer {
		private:
			glm::vec4 vector;
		public:
			VectorContainer(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
			VectorContainer(const glm::vec2& v, float z = 0.0f, float w = 0.0f);
			VectorContainer(const glm::vec3& v, float w = 0.0f);
			VectorContainer(const glm::vec4& v);
			float getX() const;
			float getY() const;
			float getZ() const;
			float getW() const;
			void setX(float x);
			void setY(float y);
			void setZ(float z);
			void setW(float w);
			bool operator==(const VectorContainer& v) const;
			bool operator!=(const VectorContainer& v) const;
			VectorContainer operator-() const;
			VectorContainer operator+(const VectorContainer& v) const;
			VectorContainer operator-(const VectorContainer& v) const;
			VectorContainer operator*(const VectorContainer& v) const;
			VectorContainer operator/(const VectorContainer& v) const;
			VectorContainer& operator+=(const VectorContainer& v);
			VectorContainer& operator-=(const VectorContainer& v);
			VectorContainer& operator*=(const VectorContainer& v);
			VectorContainer& operator/=(const VectorContainer& v);
			VectorContainer operator+(const glm::vec2 & v) const;
			VectorContainer operator-(const glm::vec2 & v) const;
			VectorContainer operator*(const glm::vec2 & v) const;
			VectorContainer operator/(const glm::vec2 & v) const;
			VectorContainer& operator+=(const glm::vec2& v);
			VectorContainer& operator-=(const glm::vec2& v);
			VectorContainer& operator*=(const glm::vec2& v);
			VectorContainer& operator/=(const glm::vec2& v);
			VectorContainer operator+(const glm::vec3 & v) const;
			VectorContainer operator-(const glm::vec3 & v) const;
			VectorContainer operator*(const glm::vec3 & v) const;
			VectorContainer operator/(const glm::vec3 & v) const;
			VectorContainer& operator+=(const glm::vec3& v);
			VectorContainer& operator-=(const glm::vec3& v);
			VectorContainer& operator*=(const glm::vec3& v);
			VectorContainer& operator/=(const glm::vec3& v);
			VectorContainer operator+(const glm::vec4 & v) const;
			VectorContainer operator-(const glm::vec4 & v) const;
			VectorContainer operator*(const glm::vec4 & v) const;
			VectorContainer operator/(const glm::vec4 & v) const;
			VectorContainer& operator+=(const glm::vec4& v);
			VectorContainer& operator-=(const glm::vec4& v);
			VectorContainer& operator*=(const glm::vec4& v);
			VectorContainer& operator/=(const glm::vec4& v);
			VectorContainer operator*(float s) const;
			VectorContainer operator/(float s) const;
			VectorContainer& operator*=(float s);
			VectorContainer& operator/=(float s);
			operator glm::vec2() const;
			operator glm::vec3() const;
			operator glm::vec4() const;
	};
}

std::ostream& operator <<(std::ostream& out, const cgl::VectorContainer& position);

namespace std {
	template<>
	struct hash<cgl::VectorContainer> {
		size_t operator()(const cgl::VectorContainer& p) const;
	};
}

#endif