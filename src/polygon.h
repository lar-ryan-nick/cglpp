#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include <list>
#include "Position.h"

class Polygon {
	std::list<glm::vec2> verticies;
	public:
		bool isInside(const Position& p) const;
		bool isInside(const glm::vec2& v) const;
		bool isInside(float x, float y) const;
		void addVertex(const glm::vec2& v);
};

#endif
