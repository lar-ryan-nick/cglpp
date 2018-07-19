#include "WorldView.h"

cgl::Shader* cgl::WorldView::worldViewShader = NULL;
float cgl::WorldView::pitch = 0.0f;
float cgl::WorldView::yaw = 90.0f;

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
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	Rectangle bounds = getBounds();
	Polygon clipArea;
	clipArea.addVertex(glm::vec2(bounds.getX(), bounds.getY()));
	clipArea.addVertex(glm::vec2(bounds.getX() + bounds.getWidth(), bounds.getY()));
	clipArea.addVertex(glm::vec2(bounds.getX() + bounds.getWidth(), bounds.getY() + bounds.getHeight()));
	clipArea.addVertex(glm::vec2(bounds.getX(), bounds.getY() + bounds.getHeight()));
	worldViewShader->use();
	worldViewShader->setUniform("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	worldViewShader->setUniform("light", spotLight);
	for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		Actor* actor = *it;
		actor->draw(*worldViewShader, parentModel * view * projection, clipArea);
	}
	worldViewShader->finish();
	glDisable(GL_DEPTH_TEST);
}

cgl::Camera cgl::WorldView::getCamera() {
	return camera;
}

void cgl::WorldView::setCamera(const Camera& c) {
	camera = c;
}

void cgl::WorldView::onMouseMove(float xOffset, float yOffset) {
	float sensitivity = 0.15f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	glm::vec3 direction = camera.getDirection();
	yaw += xOffset;
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
	pitch += yOffset;
	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	camera.setRotation(pitch, yaw);
}
