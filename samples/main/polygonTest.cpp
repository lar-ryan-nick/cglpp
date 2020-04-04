#include <iostream>
#include <Polygon.h>

int main() {
	cgl::Polygon p;
	p.addVertex(glm::vec2(0.0f, 0.0f));
	p.addVertex(glm::vec2(1.0f, 0.0f));
	p.addVertex(glm::vec2(1.0f, 1.0f));
	p.addVertex(glm::vec2(0.0f, 1.0f));
	std::cout << p.contains(glm::vec2(0.5f, 0.5f)) << std::endl;
	std::cout << p.contains(glm::vec2(1.5f, 0.5f)) << std::endl;
	return 0;
}
