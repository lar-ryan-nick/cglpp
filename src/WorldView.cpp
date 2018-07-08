#include "WorldView.h"

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height) {}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& p) {
	View::draw(parentModel, p);
	glm::mat4 model = getTransformationMatrix();
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 100.0f);
	for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {

	}
}