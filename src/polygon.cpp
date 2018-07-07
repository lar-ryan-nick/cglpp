#include "polygon.h"

cgl::Polygon::VertexNode::VertexNode(const Position& p, VertexNode* n1, VertexNode* n2) : vertex(p), next1(n1), next2(n2) {}

bool cgl::Polygon::contains(const Position& p) const {
	Position insidePoint;
	for (std::list<Position>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
		insidePoint.move(it->getX(), it->getY());
	}
	insidePoint /= static_cast<float>(verticies.size());
	for (std::list<Position>::const_iterator it1 = verticies.cbegin(); it1 != verticies.cend(); it1++) {
		std::list<Position>::const_iterator it2 = std::next(it1);
		if (it2 == verticies.cend()) {
			it2 = verticies.cbegin();
		}
		Position diff = *it2 - *it1;
		glm::vec3 insideCross = glm::cross(static_cast<glm::vec3>(insidePoint - *it1), static_cast<glm::vec3>(diff));
		glm::vec3 queryCross = glm::cross(static_cast<glm::vec3>(p - *it1), static_cast<glm::vec3>(diff));
		if (insideCross.z * queryCross.z < 0) {
			return false;
		}
	}
	return true;
}

bool cgl::Polygon::contains(const glm::vec2& v) const {
	return contains(Position(v.x, v.y));
}

bool cgl::Polygon::contains(float x, float y) const {
	return contains(Position(x, y));
}

std::list<cgl::Polygon> cgl::Polygon::clipTo(const Polygon& p) const {
	if (verticies.size() < 3 || p.verticies.size() < 3) {
		return std::list<Polygon>();
	}
	std::list<VertexNode*> intersectionPoints;
	VertexNode* clippingHead = new VertexNode(p.verticies.front());
	VertexNode* current = clippingHead;
	for (std::list<Position>::const_iterator it1 = p.verticies.cbegin(); it1 != p.verticies.cend(); it1++) {
		std::list<Position>::const_iterator it2 = std::next(it1);
		if (it2 == p.verticies.cend()) {
			it2 = p.verticies.cbegin();
		}
		Position side[2];
		side[0] = *it1;
		side[1] = *it2;
		std::list<VertexNode*> points;
		for (std::list<Position>::const_iterator it3 = verticies.cbegin(); it3 != verticies.cend(); it3++) {
			std::list<Position>::const_iterator it4 = std::next(it3);
			if (it4 == verticies.cend()) {
				it4 = verticies.cbegin();
			}
			Position edge[2];
			edge[0] = *it3;
			edge[1] = *it4;
			Position intersectionPoint = intersect(side, edge);
			if (intersectionPoint != Position(-1000000000, -1000000000)) {
				VertexNode* node = new VertexNode(intersectionPoint);
				intersectionPoints.push_back(node);
				float distance = sqrt(pow(intersectionPoint.getX() - it1->getX(), 2) + pow(intersectionPoint.getY() - it1->getY(), 2));
				bool added = false;
				for (std::list<VertexNode*>::iterator it5 = points.begin(); it5 != points.end(); it5++) {
					VertexNode* n = *it5;
					float tempDistance = sqrt(pow(n->vertex.getX() - it1->getX(), 2) + pow(n->vertex.getY() - it1->getY(), 2));
					if (distance < tempDistance) {
						points.insert(it5, node);
						added = true;
						break;
					}
				}
				if (!added) {
					points.push_back(node);
				}
			}
		}
		for (std::list<VertexNode*>::iterator it3 = points.begin(); it3 != points.end(); it3++) {
			current->next1 = *it3;
			current = current->next1;
		}
		if (it2 != p.verticies.cbegin()) {
			current->next1 = new VertexNode(*it2);
			current = current->next1;
		} else {
			current->next1 = clippingHead;
		}
	}
	clippingHead = new VertexNode(verticies.front());
	current = clippingHead;
	for (std::list<Position>::const_iterator it1 = verticies.cbegin(); it1 != verticies.cend(); it1++) {
		std::list<Position>::const_iterator it2 = std::next(it1);
		if (it2 == verticies.cend()) {
			it2 = verticies.cbegin();
		}
		Position side[2];
		side[0] = *it1;
		side[1] = *it2;
		std::list<VertexNode*> points;
		for (std::list<Position>::const_iterator it3 = p.verticies.cbegin(); it3 != p.verticies.cend(); it3++) {
			std::list<Position>::const_iterator it4 = std::next(it3);
			if (it4 == p.verticies.cend()) {
				it4 = p.verticies.cbegin();
			}
			Position edge[2];
			edge[0] = *it3;
			edge[1] = *it4;
			// do (edge, side) to preserve order from previous call and avoid float inaccuracies
			Position intersectionPoint = intersect(edge, side);
			if (intersectionPoint != Position(-1000000000, -1000000000)) {
				VertexNode* node = NULL;
				for (std::list<VertexNode*>::iterator it5 = intersectionPoints.begin(); it5 != intersectionPoints.end(); it5++) {
					VertexNode* interPoint = *it5;
					if (interPoint->vertex == intersectionPoint) {
						node = interPoint;
						break;
					}
				}
				// node should never be NULL
				float distance = sqrt(pow(intersectionPoint.getX() - it1->getX(), 2) + pow(intersectionPoint.getY() - it1->getY(), 2));
				bool added = false;
				for (std::list<VertexNode*>::iterator it5 = points.begin(); it5 != points.end(); it5++) {
					VertexNode* n = *it5;
					float tempDistance = sqrt(pow(n->vertex.getX() - it1->getX(), 2) + pow(n->vertex.getY() - it1->getY(), 2));
					if (distance < tempDistance) {
						points.insert(it5, node);
						added = true;
						break;
					}
				}
				if (!added) {
					points.push_back(node);
				}
			}
		}
		for (std::list<VertexNode*>::iterator it3 = points.begin(); it3 != points.end(); it3++) {
			if (current->next1 == NULL) {
				current->next1 = *it3;
				current = current->next1;
			} else {
				current->next2 = *it3;
				current = current->next2;
			}
		}
		if (it2 != verticies.cbegin()) {
			if (current->next1 == NULL) {
				current->next1 = new VertexNode(*it2);
				current = current->next1;
			} else {
				current->next2 = new VertexNode(*it2);
				current = current->next2;
			}
		} else {
			if (current->next1 == NULL) {
				current->next1 = clippingHead;
			} else {
				current->next2 = clippingHead;
			}
		}
	}
	int numIntersections = intersectionPoints.size();
	std::list<Polygon> mappedPolygons;
	if (numIntersections > 0) {
		current = intersectionPoints.front();
		bool adding = false, first = true;
		Polygon mappedPolygon;
		while (first || current != intersectionPoints.front()) {
			first = false;
			bool option1 = current->next2 != NULL; // is an intersection point
			bool option2 = p.contains(current->vertex) && contains(current->vertex);
			if (option1 || option2) {
				mappedPolygon.addVertex(current->vertex);
				adding = true;
			} else {
				if (adding) {
					adding = false;
					mappedPolygons.push_back(mappedPolygon);
					mappedPolygon = Polygon();
				}
			}
			if (current->next2 != NULL) {
				option1 = current->next2->next2 != NULL;
				option2 = p.contains(current->next2->vertex) && contains(current->next2->vertex);
				if (option1 || option2) {
					current = current->next2;
				} else {
					current = current->next1;
				}
			} else {
				current = current->next1;
			}
		}
		if (adding) {
			if (!mappedPolygons.empty()) { 
				Polygon p = mappedPolygons.front();
				mappedPolygons.erase(mappedPolygons.begin());
				for (std::list<Position>::iterator it = p.verticies.begin(); it != p.verticies.end(); it++) {
					mappedPolygon.addVertex(*it);
				}
			}
			mappedPolygons.push_back(mappedPolygon);
		}
		// cleanup
		int intersectionDeleteCount = 0;
		VertexNode* iterator = intersectionPoints.front();
		first = true;
		while (intersectionDeleteCount < numIntersections - 1) {
			current = iterator->next1;
			while (current->next2 == NULL) {
				VertexNode* previous = current;
				current = current->next1;
				delete previous;
			}
			current = iterator->next2;
			while (current->next2 == NULL) {
				VertexNode* previous = current;
				current = current->next1;
				delete previous;
			}
			if (!first) {
				delete iterator;
				intersectionDeleteCount++;
			}
			iterator = current;
			first = false;
		}
		delete iterator;
	} else {
		bool allInside = true;
		for (std::list<Position>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
			if (!p.contains(*it)) {
				allInside = false;
				break;
			}
		}
		if (allInside) {
			Polygon mappedPolygon;
			for (std::list<Position>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
				mappedPolygon.addVertex(*it);
			}
			mappedPolygons.push_back(mappedPolygon);
		} else {
			allInside = true;
			for (std::list<Position>::const_iterator it = p.verticies.cbegin(); it != p.verticies.cend(); it++) {
				if (!contains(*it)) {
					allInside = false;
					break;
				}
			}
			if (allInside) {
				Polygon mappedPolygon;
				for (std::list<Position>::const_iterator it = p.verticies.cbegin(); it != p.verticies.cend(); it++) {
					mappedPolygon.addVertex(*it);
				}
				mappedPolygons.push_back(mappedPolygon);
			}
		}
	}
	return mappedPolygons;
}

cgl::Position cgl::intersect(const Position v1[2], const Position v2[2]) {
	// v1 is vertical
	if (v1[1].getX() == v1[0].getX()) {
		if (v2[1].getX() != v2[0].getX() && ((v2[0].getX() <= v1[0].getX() && v1[0].getX() <= v2[1].getX()) || (v2[1].getX() <= v1[0].getX() && v1[0].getX() <= v2[0].getX()))) {
			float y = (v2[1].getY() - v2[0].getY()) / (v2[1].getX() - v2[0].getX()) * (v1[0].getX() - v2[0].getX()) + v2[0].getY();
			if (v1[0].getY() <= y && y <= v1[1].getY() || v1[1].getY() <= y && y <= v1[0].getX()) {
				return Position(v1[0].getX(), y);
			} else {
				return Position(-1000000000, -1000000000);
			}
		} else {
			return Position(-1000000000, -1000000000);
		}
	}
	// Line segments have the same slope
	if (v2[1].getY() - v2[0].getY() - (v2[1].getX() - v2[0].getX()) * (v1[1].getY() - v1[0].getY()) / (v1[1].getX() - v1[0].getX()) == 0) {
		return Position(-1000000000, -1000000000);
	}
	float j = (v1[0].getY() - v2[0].getY() + (v1[1].getY() - v1[0].getY()) * (v2[0].getX() - v1[0].getX()) / (v1[1].getX() - v1[0].getX())) / (v2[1].getY() - v2[0].getY() - (v1[1].getY() - v1[0].getY()) * (v2[1].getX() - v2[0].getX()) / (v1[1].getX() - v1[0].getX()));
	float i = (v2[0].getX() + (v2[1].getX() - v2[0].getX()) * j - v1[0].getX()) / (v1[1].getX() - v1[0].getX());
	if (i > 0 && i < 1 && j > 0 && j < 1) {
		return Position(v1[0].getX() + (v1[1].getX() - v1[0].getX()) * i, v1[0].getY() + (v1[1].getY() - v1[0].getY()) * i);
	}
	return Position(-1000000000, -1000000000);
}

void cgl::Polygon::addVertex(const glm::vec2& v) {
	if (verticies.empty() || verticies.back() != v) {
		verticies.push_back(v);
	}
}

void cgl::Polygon::addVertex(const Position& p) {
	if (verticies.empty() || verticies.back() != p) {
		verticies.push_back(p);
	}
}

std::list<cgl::Position>& cgl::Polygon::getVerticies() {
	return verticies;
}

const std::list<cgl::Position>& cgl::Polygon::getVerticies() const {
	return verticies;
}
