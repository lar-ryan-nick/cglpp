#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "position.h"

namespace cgl {
	class Polygon {
		private:
			struct VertexNode {
				glm::vec2 vertex;
				VertexNode* next1;
				VertexNode* next2;
				VertexNode(const glm::vec2& v, VertexNode* n1 = NULL, VertexNode* n2 = NULL) : vertex(v), next1(n1), next2(n2) {}
			};
			std::list<glm::vec2> verticies;
		public:
			bool isInside(const Position& p) const;
			bool isInside(const glm::vec2& v) const;
			bool isInside(float x, float y) const;
			std::list<Polygon> mapTo(const Polygon& p) const;
			void addVertex(const glm::vec2& v);
			std::list<glm::vec2>& getVerticies();
			const std::list<glm::vec2>& getVerticies() const;
	};

	glm::vec2 intersect(glm::vec2 v1[2], glm::vec2 v2[2]);
}

#endif
