#include "VectorContainer.h"

cgl::VectorContainer::VectorContainer(float x, float y, float z, float w) : vector(x, y, z, w) {}

cgl::VectorContainer::VectorContainer(const glm::vec2& v) : vector(v.x, v.y, 0.0f, 0.0f) {}

cgl::VectorContainer::VectorContainer(const glm::vec3& v) : vector(v.x, v.y, v.z, 0.0f) {}

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

bool cgl::VectorContainer::operator==(const glm::vec2& v) const {
	return vector.x == v.x && vector.y == v.y;
}

bool cgl::VectorContainer::operator!=(const glm::vec2& v) const {
	return !(*this == v);
}

bool cgl::VectorContainer::operator==(const glm::vec3& v) const {
	return vector.x == v.x && vector.y == v.y && vector.z == v.z;
}

bool cgl::VectorContainer::operator!=(const glm::vec3& v) const {
	return !(*this == v);
}

bool cgl::VectorContainer::operator==(const glm::vec4& v) const {
	return vector == v;
}

bool cgl::VectorContainer::operator!=(const glm::vec4& v) const {
	return vector != v;
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
	return vector + v;
}

cgl::VectorContainer cgl::VectorContainer::operator-(const glm::vec2 & v) const {
	return vector - v;
}

cgl::VectorContainer cgl::VectorContainer::operator*(const glm::vec2 & v) const {
	return vector * v;
}

cgl::VectorContainer cgl::VectorContainer::operator/(const glm::vec2 & v) const {
	return vector / v;
}

cgl::VectorContainer& cgl::VectorContainer::operator+=(const glm::vec2& v) {
	vector += v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator-=(const glm::vec2& v) {
	vector -= v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::vec2& v) {
	vector *= v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(const glm::vec2& v) {
	vector /= v;
	return *this;
}

cgl::VectorContainer cgl::VectorContainer::operator+(const glm::vec3 & v) const {
	return vector + v;
}

cgl::VectorContainer cgl::VectorContainer::operator-(const glm::vec3 & v) const {
	return vector - v;
}

cgl::VectorContainer cgl::VectorContainer::operator*(const glm::vec3 & v) const {
	return vector * v;
}

cgl::VectorContainer cgl::VectorContainer::operator/(const glm::vec3 & v) const {
	return vector / v;
}

cgl::VectorContainer& cgl::VectorContainer::operator+=(const glm::vec3& v) {
	vector += v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator-=(const glm::vec3& v) {
	vector -= v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator*=(const glm::vec3& v) {
	vector *= v;
	return *this;
}

cgl::VectorContainer& cgl::VectorContainer::operator/=(const glm::vec3& v) {
	vector /= v;
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

cgl::VectorContainer::operator glm::vec2() {
	return glm::vec2(vector.x, vector.y);
}

cgl::VectorContainer::operator glm::vec3() {
	return glm::vec3(vector.x, vector.y, vector.z);
}

cgl::VectorContainer::operator glm::vec4() {
	return vector;
}
/*
cgl::VectorContainer operator+(const glm::vec2 & v, const cgl::VectorContainer& vc) {
	return vc + v;
}

cgl::VectorContainer operator-(const glm::vec2 & v, const cgl::VectorContainer& vc) {
	return vc - v;
}

cgl::VectorContainer operator*(const glm::vec2 & v, const cgl::VectorContainer& vc) {
	return vc * v;
}

cgl::VectorContainer operator/(const glm::vec2 & v, const cgl::VectorContainer& vc) {
	return vc / v;
}

glm::vec2& operator+=(const glm::vec2& v, const cgl::VectorContainer& vc) {
	v = vc + v;
	return v;
}

glm::vec2& operator-=(const glm::vec2& v, const cgl::VectorContainer& vc) {
	v = vc - v;
	return v;
}

glm::vec2& operator*=(const glm::vec2& v, const cgl::VectorContainer& vc) {
	v = vc * v;
	return v;
}

glm::vec2& operator/=(const glm::vec2& v, const cgl::VectorContainer& vc) {
	v = vc / v;
	return v;
}

cgl::VectorContainer operator+(const glm::vec3 & v, const cgl::VectorContainer& vc) {
	return vc + v;
}

cgl::VectorContainer operator-(const glm::vec3 & v, const cgl::VectorContainer& vc) {
	return vc - v;
}

cgl::VectorContainer operator*(const glm::vec3 & v, const cgl::VectorContainer& vc) {
	return vc * v;
}

cgl::VectorContainer operator/(const glm::vec3 & v, const cgl::VectorContainer& vc) {
	return vc / v;
}

glm::vec3& operator+=(const glm::vec3& v, const cgl::VectorContainer& vc) {
	v = vc + v;
	return v;
}

glm::vec3& operator-=(const glm::vec3& v, const cgl::VectorContainer& vc) {
	v = vc - v;
	return v;
}

glm::vec3& operator*=(const glm::vec3& v, const cgl::VectorContainer& vc) {
	v = vc * v;
	return v;
}

glm::vec3& operator/=(const glm::vec3& v, const cgl::VectorContainer& vc) {
	v = vc / v;
	return v;
}

cgl::VectorContainer operator+(const glm::vec4 & v, const cgl::VectorContainer& vc) {
	return vc + v;
}

cgl::VectorContainer operator-(const glm::vec4 & v, const cgl::VectorContainer& vc) {
	return vc - v;
}

cgl::VectorContainer operator*(const glm::vec4 & v, const cgl::VectorContainer& vc) {
	return vc * v;
}

cgl::VectorContainer operator/(const glm::vec4 & v, const cgl::VectorContainer& vc) {
	return vc / v;
}

cgl::VectorContainer& operator+=(const glm::vec4& v, const cgl::VectorContainer& vc) {
	v = vc + v;
	return v;
}

cgl::VectorContainer& operator-=(const glm::vec4& v, const cgl::VectorContainer& vc) {
	v = vc - v;
	return v;
}

cgl::VectorContainer& operator*=(const glm::vec4& v, const cgl::VectorContainer& vc) {
	v = vc * v;
	return v;
}

cgl::VectorContainer& operator/=(const glm::vec4& v, const cgl::VectorContainer& vc) {
	v = vc / v;
	return v;
}

cgl::VectorContainer operator*(float s, const cgl::VectorContainer& vc) {
	return vc * s;
}

cgl::VectorContainer operator/(float s, const cgl::VectorContainer& vc) {
	return vc / s;
}
*/
std::ostream& operator <<(std::ostream& out, const cgl::VectorContainer& vc) {
	out << '<' << vc.getX() << ", " << vc.getY() << ", " << vc.getZ() << ", " << vc.getW() << '>' << std::endl;
	return out;
}

size_t std::hash<cgl::VectorContainer>::operator()(const cgl::VectorContainer& vc) const {
	return hash<float>()(vc.getX()) ^ hash<float>()(vc.getY()) ^ hash<float>()(vc.getZ()) ^ hash<float>()(vc.getW());
}