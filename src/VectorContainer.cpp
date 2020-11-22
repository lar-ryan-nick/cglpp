#include "VectorContainer.h"

cgl::VectorContainer::VectorContainer(float x, float y, float z, float w) : vector(x, y, z, w) {}

cgl::VectorContainer::VectorContainer(const glm::vec2& v, float z, float w) : vector(v.x, v.y, z, w) {}

cgl::VectorContainer::VectorContainer(const glm::vec3& v, float w) : vector(v.x, v.y, v.z, w) {}

cgl::VectorContainer::VectorContainer(const glm::vec4& v) : vector(v.x, v.y, v.z, v.w) {}

float cgl::VectorContainer::getX() const {
	return vector.x;
}

float cgl::VectorContainer::getY() const {
	return vector.y;
}

float cgl::VectorContainer::getZ() const {
	return vector.z;
}

float cgl::VectorContainer::getW() const {
	return vector.w;
}

void cgl::VectorContainer::setX(float x) {
	vector.x = x;
}

void cgl::VectorContainer::setY(float y) {
	vector.y = y;
}

void cgl::VectorContainer::setZ(float z) {
	vector.z = z;
}

void cgl::VectorContainer::setW(float w) {
	vector.w = w;
}
bool cgl::VectorContainer::operator==(const VectorContainer& v) const {
	return vector == v.vector;
}

bool cgl::VectorContainer::operator!=(const VectorContainer& v) const {
	return vector != v.vector;
}

cgl::VectorContainer cgl::VectorContainer::operator-() const {
	return -vector;
}

cgl::VectorContainer cgl::VectorContainer::operator+(const VectorContainer& v) const {
	return vector + v.vector;
}

cgl::VectorContainer cgl::VectorContainer::operator-(const VectorContainer& v) const {
	return vector - v.vector;
}

cgl::VectorContainer cgl::VectorContainer::operator*(const VectorContainer& v) const {
	return vector * v.vector;
}

cgl::VectorContainer cgl::VectorContainer::operator/(const VectorContainer& v) const {
	return vector / v.vector;
}

cgl::VectorContainer& cgl::VectorContainer::operator+=(const VectorContainer& v) {
	vector += v.vector;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator-=(const VectorContainer& v) {
	vector -= v.vector;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const VectorContainer& v) {
	vector *= v.vector;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(const VectorContainer& v) {
	vector /= v.vector;
	return *this;
}

cgl::VectorContainer cgl::VectorContainer::operator+(const glm::vec2 & v) const {
	return vector + glm::vec4(v, 0.0f, 0.0f);
}

cgl::VectorContainer cgl::VectorContainer::operator-(const glm::vec2 & v) const {
	return vector - glm::vec4(v, 0.0f, 0.0f);
}

cgl::VectorContainer cgl::VectorContainer::operator*(const glm::vec2 & v) const {
	return vector * glm::vec4(v, 1.0f, 1.0f);
}

cgl::VectorContainer cgl::VectorContainer::operator/(const glm::vec2 & v) const {
	return vector / glm::vec4(v, 1.0f, 1.0f);
}

cgl::VectorContainer& cgl::VectorContainer::operator+=(const glm::vec2& v) {
	vector += glm::vec4(v, 0.0f, 0.0f);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator-=(const glm::vec2& v) {
	vector -= glm::vec4(v, 0.0f, 0.0f);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::vec2& v) {
	vector *= glm::vec4(v, 1.0f, 1.0f);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(const glm::vec2& v) {
	vector /= glm::vec4(v, 1.0f, 1.0f);
	return *this;
}

cgl::VectorContainer cgl::VectorContainer::operator+(const glm::vec3 & v) const {
	return vector + glm::vec4(v, 0.0f);
}

cgl::VectorContainer cgl::VectorContainer::operator-(const glm::vec3 & v) const {
	return vector - glm::vec4(v, 0.0f);
}

cgl::VectorContainer cgl::VectorContainer::operator*(const glm::vec3 & v) const {
	return vector * glm::vec4(v, 1.0f);
}

cgl::VectorContainer cgl::VectorContainer::operator/(const glm::vec3 & v) const {
	return vector / glm::vec4(v, 1.0f);
}

cgl::VectorContainer& cgl::VectorContainer::operator+=(const glm::vec3& v) {
	vector += glm::vec4(v, 0.0f);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator-=(const glm::vec3& v) {
	vector -= glm::vec4(v, 0.0f);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::vec3& v) {
	vector *= glm::vec4(v, 1.0f);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(const glm::vec3& v) {
	vector /= glm::vec4(v, 1.0f);
	return *this;
}

cgl::VectorContainer cgl::VectorContainer::operator+(const glm::vec4 & v) const {
	return vector + v;
}

cgl::VectorContainer cgl::VectorContainer::operator-(const glm::vec4 & v) const {
	return vector - v;
}

cgl::VectorContainer cgl::VectorContainer::operator*(const glm::vec4 & v) const {
	return vector * v;
}

cgl::VectorContainer cgl::VectorContainer::operator/(const glm::vec4 & v) const {
	return vector / v;
}

cgl::VectorContainer& cgl::VectorContainer::operator+=(const glm::vec4& v) {
	vector += v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator-=(const glm::vec4& v) {
	vector -= v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::vec4& v) {
	vector *= v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(const glm::vec4& v) {
	vector /= v;
	return *this;
}
cgl::VectorContainer cgl::VectorContainer::operator*(float s) const {
	return vector * s;
}

cgl::VectorContainer cgl::VectorContainer::operator/(float s) const {
	return vector / s;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(float s) {
	vector *= s;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(float s) {
	vector /= s;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::mat2& m) {
	vector = glm::vec4(static_cast<glm::vec2>(vector) * m, vector.z, vector.w);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::mat3& m) {
	vector = glm::vec4(static_cast<glm::vec3>(vector) * m, vector.w);
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::mat4& m) {
	vector = vector * m;
	return *this;
}

cgl::VectorContainer::operator glm::vec2() const {
	return glm::vec2(vector);
}

cgl::VectorContainer::operator glm::vec3() const {
	return glm::vec3(vector);
}

cgl::VectorContainer::operator glm::vec4() const {
	return vector;
}

cgl::VectorContainer operator*(const cgl::VectorContainer& vc, const glm::mat2& m) {
	return cgl::VectorContainer(static_cast<glm::vec2>(vc) * m, vc.getZ(), vc.getW());
}

cgl::VectorContainer operator*(const glm::mat2& m, const cgl::VectorContainer& vc) {
	return cgl::VectorContainer(m * static_cast<glm::vec2>(vc), vc.getZ(), vc.getW());
}

cgl::VectorContainer operator*(const cgl::VectorContainer& vc, const glm::mat3& m) {
	return cgl::VectorContainer(static_cast<glm::vec3>(vc) * m, vc.getW());
}

cgl::VectorContainer operator*(const glm::mat3& m, const cgl::VectorContainer& vc) {
	return cgl::VectorContainer(m * static_cast<glm::vec3>(vc), vc.getW());
}

cgl::VectorContainer operator*(const cgl::VectorContainer& vc, const glm::mat4& m) {
	return static_cast<glm::vec4>(vc) * m;
}

cgl::VectorContainer operator*(const glm::mat4& m, const cgl::VectorContainer& vc) {
	return m * static_cast<glm::vec4>(vc);
}

std::ostream& operator<<(std::ostream& out, const cgl::VectorContainer& vc) {
	out << '<' << vc.getX() << ", " << vc.getY() << ", " << vc.getZ() << ", " << vc.getW() << '>';
	return out;
}

size_t std::hash<cgl::VectorContainer>::operator()(const cgl::VectorContainer& vc) const {
	return hash<float>()(vc.getX()) ^ hash<float>()(vc.getY()) ^ hash<float>()(vc.getZ()) ^ hash<float>()(vc.getW());
}
