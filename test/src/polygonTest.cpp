#include <iostream>
#include "../../src/polygon.h"

int main() {
	Polygon p;
	p.addVertex(glm::vec2(0.0f, 0.0f));
	p.addVertex(glm::vec2(1.0f, 0.0f));
	p.addVertex(glm::vec2(1.0f, 1.0f));
	p.addVertex(glm::vec2(0.0f, 1.0f));
	std::cout << p.isInside(glm::vec2(0.5f, 0.5f)) << std::endl;
	std::cout << p.isInside(glm::vec2(1.5f, 0.5f)) << std::endl;
	return 0;
}
