#include "Polygon.h"

cgl::Polygon::VertexNode::VertexNode(const Position& p, VertexNode* n1, VertexNode* n2, bool e) : vertex(p), next1(n1), next2(n2), exiting(e) {}

bool cgl::Polygon::contains(const Position& p) const {
	Position insidePoint;
	for (std::list<Position>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
		insidePoint += *it;
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
	std::list<VertexNode*> exitingIntersections;
	VertexNode* clippingHead = new VertexNode(p.verticies.front());
	VertexNode* current = clippingHead;
	std::unordered_map<Position, std::list<VertexNode*> > intersections;
	for (std::list<Position>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
		intersections.insert(std::pair<Position, std::list<VertexNode*> >(*it, std::list<VertexNode*>()));
	}
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
				std::list<VertexNode*>& inters = intersections.find(*it3)->second;
				LineSegment intersectSegment(intersectionPoint, *it3);
				added = false;
				for (std::list<VertexNode*>::iterator it5 = inters.begin(); it5 != inters.end(); it5++) {
					VertexNode* n = *it5;
					LineSegment other(n->vertex, *it3);
					if (intersectSegment.getLength() < other.getLength()) {
						inters.insert(it5, node);
						added = true;
						break;
					}
				}
				if (!added) {
					inters.push_back(node);
				}
			}
		}
		bool exitingPoly;
		if (!points.empty()) {
			exitingPoly = contains(*it1);
		}
		for (std::list<VertexNode*>::iterator it3 = points.begin(); it3 != points.end(); it3++) {
			current->next1 = *it3;
			current = current->next1;
			current->exiting = exitingPoly;
			if (current->exiting) {
				exitingIntersections.push_back(current);
			}
			exitingPoly = !exitingPoly;
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
		std::list<VertexNode*>& points = intersections.find(*it1)->second;
		for (std::list<VertexNode*>::iterator it3 = points.begin(); it3 != points.end(); it3++) {
			current->next2 = *it3;
			current = current->next2;
		}
		if (it2 != verticies.begin()) {
			current->next2 = new VertexNode(*it2);
			current = current->next2;
		} else {
			current->next2 = clippingHead;
		}
	}
	std::list<Polygon> mappedPolygons;
	if (exitingIntersections.size() > 0) {
		for (std::list<VertexNode*>::iterator it = exitingIntersections.begin(); it != exitingIntersections.end(); it++) {
			VertexNode* start = *it;
			if (start->exiting) {
				Polygon mappedPolygon;
				mappedPolygon.addVertex(start->vertex);
				bool tracer = false;
				VertexNode* current = start->next2;
				while (current != start) {
					mappedPolygon.addVertex(current->vertex);
					// intersection point
					if (current->next1 && current->next2) {
						tracer = !tracer;
						current->exiting = false;
					}
					if (tracer) {
						current = current->next1;
					}
					else {
						current = current->next2;
					}
				}
				mappedPolygons.push_back(mappedPolygon);
			}
		}
		// cleanup
		int intersectionDeleteCount = 0;
		VertexNode* iterator = exitingIntersections.front();
		bool first = true;
		while (intersectionDeleteCount < exitingIntersections.size() - 1) {
			current = iterator->next1;
			while (current->next2 == NULL) {
				VertexNode* previous = current;
				current = current->next1;
				delete previous;
			}
			current = iterator->next2;
			while (current->next1 == NULL) {
				VertexNode* previous = current;
				current = current->next2;
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
