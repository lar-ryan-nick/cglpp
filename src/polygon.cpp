#include "polygon.h"

bool Polygon::isInside(const Position& p) const {
	return isInside(glm::vec2(p.getX(), p.getY()));
}

bool Polygon::isInside(const glm::vec2& v) const {
	glm::vec3 insidePoint;
	for (std::list<glm::vec2>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
		insidePoint.x += it->x;
		insidePoint.y += it->y;
	}
	insidePoint.x /= verticies.size();
	insidePoint.y /= verticies.size();
	glm::vec3 queryPoint(v.x, v.y, 0.0f);
	std::list<glm::vec2>::const_iterator it = verticies.cbegin();
	glm::vec3 diff = glm::vec3(*it - verticies.back(), 0.0f);
	glm::vec3 insideCross = glm::cross(insidePoint - glm::vec3(verticies.back(), 0.0f), diff);
	glm::vec3 queryCross = glm::cross(queryPoint - glm::vec3(verticies.back(), 0.0f), diff);
	if (insideCross.z * queryCross.z < 0) {
		return false;
	}
	it++;
	while (it != verticies.cend()) {
		diff = glm::vec3(*it - *(std::next(it, -1)), 0.0f);
		insideCross = glm::cross(insidePoint - glm::vec3(*(std::next(it, -1)), 0.0f), diff);
		queryCross = glm::cross(queryPoint - glm::vec3(*(std::next(it, -1)), 0.0f), diff);
		if (insideCross.z * queryCross.z < 0) {
			return false;
		}
		it++;
	}
	return true;
}

bool Polygon::isInside(float x, float y) const {
	return isInside(glm::vec2(x, y));
}

void Polygon::addVertex(const glm::vec2& v) {
	verticies.push_back(v);
}
