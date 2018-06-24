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
	VertexNode* clippingHead = NULL;
	std::vector<VertexNode*> originalNodes;
	int numIntersections = 0;
	for (std::list<glm::vec2>::const_iterator it = verticies.cbegin(); it != verticies.cend(); it++) {
		originalNodes.push_back(new VertexNode(*it));
	}
	for (int i = 0; i < originalNodes.size(); i++) {
		originalNodes[i]->next1 = originalNodes[(i + 1) % originalNodes.size()];
	}
	VertexNode* current = new VertexNode(p.verticies.front());
	for (std::list<glm::vec2>::const_iterator it1 = p.verticies.cbegin(); it1 != p.verticies.cend(); it1++) {
		std::list<glm::vec2>::const_iterator it2 = std::next(it1);
		if (it2 == verticies.cend()) {
			it2 = verticies.cbegin();
		}
		glm::vec2 side[2];
		side[0] = *it1;
		side[1] = *it2;
		std::list<VertexNode*> intersectionPoints;
		for (int i = 0; i < originalNodes.size(); i++) {
			glm::vec2 edge[2];
			edge[0] = originalNodes[i]->vertex;
			edge[1] = originalNodes[(i + 1) % originalNodes.size()]->vertex;
			glm::vec2 intersectionPoint = intersect(side, edge);
			if (intersectionPoint != glm::vec2(-1000000000, -1000000000)) {
				numIntersections++;
				VertexNode* node = new VertexNode(intersectionPoint, NULL, originalNodes[(i + 1) % originalNodes.size()]);
				originalNodes[i]->next1 = node;
				float distance = sqrt(pow(intersectionPoint.x - it1->x, 2) + pow(intersectionPoint.y - it1->y, 2));
				bool added = false;
				for (std::list<VertexNode*>::iterator it3 = intersectionPoints.begin(); it3 != intersectionPoints.end(); it3++) {
					VertexNode* n = *it3;
					float tempDistance = sqrt(pow(n->vertex.x - it1->x, 2) + pow(n->vertex.y - it1->y, 2));
					if (distance < tempDistance) {
						intersectionPoints.insert(it3, node);
						added = true;
						break;
					}
				}
				if (!added) {
					intersectionPoints.push_back(node);
				}
			}
		}
		if (clippingHead == NULL) {
			clippingHead = current;
		}
		for (std::list<VertexNode*>::iterator it3 = intersectionPoints.begin(); it3 != intersectionPoints.end(); it3++) {
			current->next1 = *it3;
			current = current->next1;
		}
		current->next1 = new VertexNode(*it2, clippingHead);
		current = current->next1;
	}
	std::list<Polygon> mappedPolygons;
	if (numIntersections > 0) {
		VertexNode* iterator = originalNodes[0];
		while (iterator->next2 == NULL) {
			iterator = iterator->next1;
		}
		bool adding = false;
		int intersectionCount = 0;
		Polygon mappedPolygon;
		while (intersectionCount <= numIntersections) {
			bool option1 = iterator->next2 != NULL;
			bool option2;
			if (std::find(originalNodes.begin(), originalNodes.end(), iterator) != originalNodes.end()) {
				option2 = p.isInside(iterator->vertex);
			} else {
				option2 = isInside(iterator->vertex);
			}
			if (option1 || option2) {
				if (option1) {
					intersectionCount++;
				}
				mappedPolygon.addVertex(iterator->vertex);
				adding = true;
			} else {
				if (adding) {
					adding = false;
				}
				mappedPolygons.push_back(mappedPolygon);
				mappedPolygon = Polygon();
			}
			if (iterator->next2 != NULL) {
				option1 = iterator->next2->next2 != NULL;
				if (std::find(originalNodes.begin(), originalNodes.end(), iterator->next2) != originalNodes.end()) {
					option2 = p.isInside(iterator->next2->vertex);
				} else {
					option2 = isInside(iterator->next2->vertex);
				}
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
		// cleanup
		iterator = originalNodes[0];
		int deleteCount = 0;
		VertexNode* previous = NULL;
		while (deleteCount < originalNodes.size()) {
			previous = iterator;
			if (iterator->next2 != NULL) {
				iterator = iterator->next2;
			} else {
				iterator = iterator->next1;
				delete previous;
				deleteCount++;
			}
		}
		iterator = clippingHead;
		deleteCount = 0;
		while (deleteCount < p.verticies.size() + numIntersections) {
			previous = iterator;
			iterator = iterator->next1;
			delete previous;
			deleteCount++;
		}
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
	if (std::find(verticies.begin(), verticies.end(), v) == verticies.end()) {
		verticies.push_back(v);
	}
}

std::list<glm::vec2> Polygon::getVerticies() {
	return verticies;
}
