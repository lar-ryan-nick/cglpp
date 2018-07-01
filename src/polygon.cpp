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
	if (insideCross.z * queryCross.z < -0.00001) {
		return false;
	}
	it++;
	while (it != verticies.cend()) {
		diff = glm::vec3(*it - *(std::next(it, -1)), 0.0f);
		insideCross = glm::cross(insidePoint - glm::vec3(*(std::next(it, -1)), 0.0f), diff);
		queryCross = glm::cross(queryPoint - glm::vec3(*(std::next(it, -1)), 0.0f), diff);
		if (insideCross.z * queryCross.z < -0.00001) {
			return false;
		}
		it++;
	}
	return true;
}

std::list<Polygon> Polygon::mapTo(const Polygon& p) const {
	std::list<VertexNode*> intersectionPoints;
	VertexNode* clippingHead = new VertexNode(p.verticies.front());
	VertexNode* current = clippingHead;
	for (std::list<glm::vec2>::const_iterator it1 = p.verticies.cbegin(); it1 != p.verticies.cend(); it1++) {
		std::list<glm::vec2>::const_iterator it2 = std::next(it1);
		if (it2 == p.verticies.cend()) {
			it2 = p.verticies.cbegin();
		}
		glm::vec2 side[2];
		side[0] = *it1;
		side[1] = *it2;
		std::list<VertexNode*> points;
		for (std::list<glm::vec2>::const_iterator it3 = verticies.cbegin(); it3 != verticies.cend(); it3++) {
			std::list<glm::vec2>::const_iterator it4 = std::next(it3);
			if (it4 == verticies.cend()) {
				it4 = verticies.cbegin();
			}
			glm::vec2 edge[2];
			edge[0] = *it3;
			edge[1] = *it4;
			glm::vec2 intersectionPoint = intersect(side, edge);
			if (intersectionPoint != glm::vec2(-1000000000, -1000000000)) {
				VertexNode* node = new VertexNode(intersectionPoint);
				intersectionPoints.push_back(node);
				float distance = sqrt(pow(intersectionPoint.x - it1->x, 2) + pow(intersectionPoint.y - it1->y, 2));
				bool added = false;
				for (std::list<VertexNode*>::iterator it5 = points.begin(); it5 != points.end(); it5++) {
					VertexNode* n = *it5;
					float tempDistance = sqrt(pow(n->vertex.x - it1->x, 2) + pow(n->vertex.y - it1->y, 2));
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
		current->next1 = new VertexNode(*it2, clippingHead);
		current = current->next1;
	}
	clippingHead = new VertexNode(verticies.front());
	current = clippingHead;
	for (std::list<glm::vec2>::const_iterator it1 = verticies.cbegin(); it1 != verticies.cend(); it1++) {
		std::list<glm::vec2>::const_iterator it2 = std::next(it1);
		if (it2 == verticies.cend()) {
			it2 = verticies.cbegin();
		}
		glm::vec2 side[2];
		side[0] = *it1;
		side[1] = *it2;
		std::list<VertexNode*> points;
		for (std::list<glm::vec2>::const_iterator it3 = p.verticies.cbegin(); it3 != p.verticies.cend(); it3++) {
			std::list<glm::vec2>::const_iterator it4 = std::next(it3);
			if (it4 == p.verticies.cend()) {
				it4 = p.verticies.cbegin();
			}
			glm::vec2 edge[2];
			edge[0] = *it3;
			edge[1] = *it4;
			glm::vec2 intersectionPoint = intersect(side, edge);
			if (intersectionPoint != glm::vec2(-1000000000, -1000000000)) {
				VertexNode* node = NULL;
				for (std::list<VertexNode*>::iterator it5 = intersectionPoints.begin(); it5 != intersectionPoints.end(); it5++) {
					VertexNode* interPoint = *it5;
					if (std::abs(interPoint->vertex.x - intersectionPoint.x) < 0.00001 && std::abs(interPoint->vertex.y - intersectionPoint.y) < 0.00001) {
						node = interPoint;
						break;
					}
				}
				float distance = sqrt(pow(intersectionPoint.x - it1->x, 2) + pow(intersectionPoint.y - it1->y, 2));
				bool added = false;
				for (std::list<VertexNode*>::iterator it5 = points.begin(); it5 != points.end(); it5++) {
					VertexNode* n = *it5;
					float tempDistance = sqrt(pow(n->vertex.x - it1->x, 2) + pow(n->vertex.y - it1->y, 2));
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
			if (current->next1 == NULL || current->next1 == clippingHead) {
				current->next1 = *it3;
				current = current->next1;
			} else {
				current->next2 = *it3;
				current = current->next2;
			}
		}
		if (current->next1 == NULL || current->next1 == clippingHead) {
			current->next1 = new VertexNode(*it2, clippingHead);
			current = current->next1;
		} else {
			current->next2 = new VertexNode(*it2, clippingHead);
			current = current->next2;
		}
	}
	int numIntersections = intersectionPoints.size();
	std::list<Polygon> mappedPolygons;
	if (numIntersections > 0) {
		VertexNode* iterator = intersectionPoints.front();
		bool adding = false, first = true;
		Polygon mappedPolygon;
		while (first || iterator != intersectionPoints.front()) {
			first = false;
			bool option1 = iterator->next2 != NULL; // is an intersection point
			bool option2 = p.isInside(iterator->vertex) && isInside(iterator->vertex);
			/*
			if (std::find(originalNodes.begin(), originalNodes.end(), iterator) != originalNodes.end()) {
				option2 = p.isInside(iterator->vertex); // is vertex of this and inside clipping region
			} else {
				option2 = isInside(iterator->vertex); // is vertex of clipping region and inside this
			}
			*/
			if (option1 || option2) {
				mappedPolygon.addVertex(iterator->vertex);
				adding = true;
			} else {
				if (adding) {
					adding = false;
					mappedPolygons.push_back(mappedPolygon);
					mappedPolygon = Polygon();
				}
			}
			if (iterator->next2 != NULL) { // && iterator->next2->next2 == NULL) {
				option1 = iterator->next2->next2 != NULL;
				option2 = p.isInside(iterator->next2->vertex) && isInside(iterator->next2->vertex);
				/*
				if (std::find(originalNodes.begin(), originalNodes.end(), iterator->next2) != originalNodes.end()) {
					option2 = p.isInside(iterator->next2->vertex);
				} else {
					option2 = isInside(iterator->next2->vertex);
				}
				*/
				if (option1 || option2) {
					iterator = iterator->next2;
				} else {
					iterator = iterator->next1;
				}
			} else {
				iterator = iterator->next1;
			}
		}
		if (adding) {
			if (!mappedPolygons.empty()) { 
				Polygon p = mappedPolygons.front();
				mappedPolygons.erase(mappedPolygons.begin());
				for (std::list<glm::vec2>::iterator it = p.verticies.begin(); it != p.verticies.end(); it++) {
					mappedPolygon.addVertex(*it);
				}
			}
			mappedPolygons.push_back(mappedPolygon);
		}
		/*
		// cleanup
		VertexNode* iterator1 = originalNodes[0];
		VertexNode* iterator2 = originalNodes[0];
		int deleteCount = 0;
		VertexNode* previous1 = NULL;
		VertexNode* previous2 = NULL;
		while (deleteCount < originalNodes.size()) {
			if (iterator1->next2 != NULL) {
				iterator2 = iterator1->next2;
				while (iterator1->next2 != NULL || deleteCount < originalNodes.size()) {
					previous1 = iterator1;
					iterator1 = iterator1->next1;
					delete previous1;
					deleteCount++;
				}
				while (iterator2 != iterator1 || deleteCount < originalNodes.size()) {
					previous2 = iterator2;
					iterator2 = iterator2->next1;
					delete previous2;
					deleteCount++;
				}
			} else {
				previous1 = iterator1;
				iterator1 = iterator1->next1;
				delete previous1;
				deleteCount++;
			}
		}
		*/
	} else {
		bool allInside = true;
		for (std::list<glm::vec2>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
			if (!p.isInside(*it)) {
				allInside = false;
				break;
			}
		}
		if (allInside) {
			Polygon mappedPolygon;
			for (std::list<glm::vec2>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
				mappedPolygon.addVertex(*it);
			}
			mappedPolygons.push_back(mappedPolygon);
		} else {
			allInside = true;
			for (std::list<glm::vec2>::const_iterator it = p.verticies.cbegin(); it != p.verticies.cend(); it++) {
				if (!isInside(*it)) {
					allInside = false;
					break;
				}
			}
			if (allInside) {
				Polygon mappedPolygon;
				for (std::list<glm::vec2>::const_iterator it = p.verticies.cbegin(); it != p.verticies.cend(); it++) {
					mappedPolygon.addVertex(*it);
				}
				mappedPolygons.push_back(mappedPolygon);
			}
		}
	}
	return mappedPolygons;
}

glm::vec2 intersect(glm::vec2 v1[2], glm::vec2 v2[2]) {
	float j = (v1[0].y - v2[0].y + (v1[1].y - v1[0].y) * (v2[0].x - v1[0].x) / (v1[1].x - v1[0].x)) / (v2[1].y - v2[0].y - (v1[1].y - v1[0].y) * (v2[1].x - v2[0].x) / (v1[1].x - v1[0].x));
	float i = (v2[0].x + (v2[1].x - v2[0].x) * j - v1[0].x) / (v1[1].x - v1[0].x);
	if (i >= 0 && i <= 1 && j >= 0 && j <= 1) {
		return glm::vec2(v1[0].x + (v1[1].x - v1[0].x) * i, v1[0].y + (v1[1].y - v1[0].y) * i);
	}
	return glm::vec2(-1000000000, -1000000000);
}

bool Polygon::isInside(float x, float y) const {
	return isInside(glm::vec2(x, y));
}

void Polygon::addVertex(const glm::vec2& v) {
	if (verticies.back() != v) {
		verticies.push_back(v);
	}
}

std::list<glm::vec2> Polygon::getVerticies() {
	return verticies;
}
