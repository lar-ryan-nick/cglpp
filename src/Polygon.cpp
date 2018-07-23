#include "Polygon.h"

cgl::Polygon::VertexNode::VertexNode(const Position& p, VertexNode* n1, VertexNode* n2) : vertex(p), next1(n1), next2(n2) {}

bool cgl::Polygon::contains(const Position& p) const {
	Position insidePoint;
	for (std::list<Position>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
		insidePoint.translate(it->getX(), it->getY());
	}
	insidePoint /= static_cast<float>(verticies.size());
	for (std::list<Position>::const_iterator it1 = verticies.cbegin(); it1 != verticies.cend(); it1++) {
		std::list<Position>::const_iterator it2 = std::next(it1);
		if (it2 == verticies.cend()) {
			it2 = verticies.cbegin();
		}
		glm::vec2 diff = *it2 - *it1;
		glm::vec3 insideCross = glm::cross(glm::vec3(static_cast<glm::vec2>(insidePoint - *it1), 0.0f), glm::vec3(diff, 0.0f));
		glm::vec3 queryCross = glm::cross(glm::vec3(static_cast<glm::vec2>(p - *it1), 0.0f), glm::vec3(diff, 0.0f));
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

// Weiler-Atherton Clipping Algorithm
std::list<cgl::Polygon> cgl::Polygon::clipTo(const Polygon& p) const {
	if (verticies.size() < 3 || p.verticies.size() < 3) {
		return std::list<Polygon>();
	}
	std::unordered_map<Position, VertexNode*> intersectionPoints;
	VertexNode* clippingHead = new VertexNode(p.verticies.front());
	VertexNode* current = clippingHead;
	for (std::list<Position>::const_iterator it1 = p.verticies.cbegin(); it1 != p.verticies.cend(); it1++) {
		std::list<Position>::const_iterator it2 = std::next(it1);
		if (it2 == p.verticies.cend()) {
			it2 = p.verticies.cbegin();
		}
		LineSegment side(*it1, *it2);
		std::list<VertexNode*> points;
		for (std::list<Position>::const_iterator it3 = verticies.cbegin(); it3 != verticies.cend(); it3++) {
			std::list<Position>::const_iterator it4 = std::next(it3);
			if (it4 == verticies.cend()) {
				it4 = verticies.cbegin();
			}
			LineSegment edge(*it3, *it4);
			Position intersectionPoint;
			bool intersect = edge.intersects(side, intersectionPoint);
			if (intersect) {
				VertexNode* node = new VertexNode(intersectionPoint);
				intersectionPoints.insert(std::pair<Position, VertexNode*>(intersectionPoint, node));
				LineSegment intersectionSegment(intersectionPoint, *it1);
				bool added = false;
				for (std::list<VertexNode*>::iterator it5 = points.begin(); it5 != points.end(); it5++) {
					VertexNode* n = *it5;
					LineSegment other(n->vertex, *it1);
					if (intersectionSegment.getLength() < other.getLength()) {
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
		LineSegment edge(*it1, *it2);
		std::list<VertexNode*> points;
		for (std::list<Position>::const_iterator it3 = p.verticies.cbegin(); it3 != p.verticies.cend(); it3++) {
			std::list<Position>::const_iterator it4 = std::next(it3);
			if (it4 == p.verticies.cend()) {
				it4 = p.verticies.cbegin();
			}
			LineSegment side(*it3, *it4);
			// purposely switched side and edge above so float inaccuracies would not cause issues
			Position intersectionPoint;
			bool intersect = edge.intersects(side, intersectionPoint);
			if (intersect) {
				// should always be found TODO: add check to find just in case
				VertexNode* node = intersectionPoints.find(intersectionPoint)->second;
				LineSegment intersectionSegment(intersectionPoint, *it1);
				bool added = false;
				for (std::list<VertexNode*>::iterator it5 = points.begin(); it5 != points.end(); it5++) {
					VertexNode* n = *it5;
					LineSegment other(n->vertex, *it1);
					if (intersectionSegment.getLength() < other.getLength()) {
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
		current = intersectionPoints.begin()->second;
		bool adding = false, first = true;
		Polygon mappedPolygon;
		while (first || current != intersectionPoints.begin()->second) {
			first = false;
			bool option1 = current->next2 != NULL; // is an intersection point
			bool option2 = p.contains(static_cast<glm::vec2>(current->vertex)) && contains(static_cast<glm::vec2>(current->vertex));
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
				option2 = p.contains(static_cast<glm::vec2>(current->next2->vertex)) && contains(static_cast<glm::vec2>(current->next2->vertex));
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
		VertexNode* iterator = intersectionPoints.begin()->second;
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
