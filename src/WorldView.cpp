#include "WorldView.h"

cgl::Shader* cgl::WorldView::worldViewShader = NULL;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height), camera(glm::vec3(0, 0, -3)) {
	if (worldViewShader == NULL) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl");
	}
}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& p) {
	View::draw(parentModel, p);
	//glm::mat4 model = getTransformationMatrix();
	glm::mat4 model;
	//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 view = camera.getViewMatrix();
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 100.0f);
	cgl::SpotLight spotLight;
	spotLight.setPosition(camera.getPosition());
	spotLight.setDirection(camera.getDirection());
	std::cout << Position(projection * view * model * glm::vec4(0.0f, 12.0f, 0.0f, 1.0f)) << std::endl;
	worldViewShader->use();
	worldViewShader->setUniform("model", 1, false, glm::value_ptr(model));
	worldViewShader->setUniform("view", 1, false, glm::value_ptr(view));
	worldViewShader->setUniform("projection", 1, false, glm::value_ptr(projection));
	worldViewShader->setUniform("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	worldViewShader->setUniform("light", spotLight);
	for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		Actor* actor = *it;
		actor->draw(*worldViewShader);
	}
	worldViewShader->finish();
}

cgl::Camera cgl::WorldView::getCamera() {
	return camera;
}

void cgl::WorldView::setCamera(const Camera& c) {
	camera = c;
}
