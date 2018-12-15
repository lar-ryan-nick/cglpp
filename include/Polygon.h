#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "Position.h"
#include "LineSegment.h"

namespace cgl {
	class Polygon {
		private:
			struct VertexNode {
				Position vertex;
				VertexNode* next1;
				VertexNode* next2;
				bool exiting;
				VertexNode(const Position& p, VertexNode* n1 = NULL, VertexNode* n2 = NULL, bool e = false);
			};
			std::list<Position> verticies;
		public:
			bool contains(const Position& p) const;
			bool contains(const glm::vec2& v) const;
			bool contains(float x, float y) const;
			std::list<Polygon> clipTo(const Polygon& p) const;
			void addVertex(const glm::vec2& v);
			void addVertex(const Position& p);
			void addVertex(float x, float y);
			std::list<Position>& getVerticies();
			const std::list<Position>& getVerticies() const;
	};
}

#endif
