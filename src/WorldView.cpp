#include "WorldView.h"

cgl::Shader* cgl::WorldView::worldViewShader = NULL;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height) {
	if (worldViewShader == NULL) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl");
	}
	shader = worldViewShader;
}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& p) {
	View::draw(parentModel, p);
	glm::mat4 model = getTransformationMatrix();
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 100.0f);
	cgl::SpotLight spotLight;
	spotLight.setPosition(camera.getPosition());
	spotLight.setDirection(camera.getDirection());
	shader->use();
	shader->setUniform("model", 1, false, glm::value_ptr(model));
	shader->setUniform("view", 1, false, glm::value_ptr(view));
	shader->setUniform("projection", 1, false, glm::value_ptr(projection));
	shader->setUniform("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shader->setUniform("light", spotLight);
	for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		Actor* actor = *it;
		actor->draw(*shader);
	}
}