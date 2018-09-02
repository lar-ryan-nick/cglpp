#include "WorldView.h"

cgl::Shader* cgl::WorldView::worldViewShader = NULL;
float cgl::WorldView::pitch = 0.0f;
float cgl::WorldView::yaw = 90.0f;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height), camera(glm::vec3(0.0f, 0.0f, -3.0f)) {
	if (worldViewShader == NULL) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl", "res/glsl/WorldGeometryShader.glsl");
	}
}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& poly) {
	View::draw(parentModel, poly);
	//glm::mat4 model = getTransformationMatrix();
	Rectangle bounds = getBounds();
	float verticies[8];
	verticies[0] = verticies[6] = bounds.getX();
	verticies[1] = verticies[3] = bounds.getY();
	verticies[2] = verticies[4] = bounds.getX() + bounds.getWidth();
	verticies[5] = verticies[7] = bounds.getY() + bounds.getHeight();
	glm::mat4 model = getTransformationMatrix();
	model = parentModel * model;
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f, -0.1f, 0.1f);
	glm::mat4 mvp = projection * model;
	Polygon p;
	for (int i = 0; i < 8; i += 2) {
		glm::vec4 ver(verticies[i], verticies[i + 1], 0.0f, 1.0f);
		glm::vec4 transformed = mvp * ver;
		verticies[i] = transformed.x;
		verticies[i + 1] = transformed.y;
		p.addVertex(glm::vec2(transformed.x, transformed.y));
	}
	cgl::SpotLight spotLight;
	spotLight.setPosition(camera.getPosition());
	spotLight.setDirection(camera.getDirection());
	glm::mat4 m;
	m = glm::translate(m, glm::vec3(0.0f, 0.0f, -1.75f));
	m = glm::scale(m, glm::vec3(0.2f, 0.2f, 0.2f));
	//m = model * m;
	glm::mat4 view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 100.0f);
	glm::mat4 vp = projection * view;
	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		std::list<Position> vert = it1->getVerticies();
		if (vert.size() < 3) {
			continue;
		}
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		worldViewShader->use();
		worldViewShader->setUniform("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		worldViewShader->setUniform("light", spotLight);
		worldViewShader->setUniform("vp", vp);
		worldViewShader->setUniform("model", m);
		int i = 0;
		for (std::list<Position>::iterator it2 = vert.begin(); it2 != vert.end(); it2++) {
			std::stringstream ss;
			ss << "clipRegion[" << i << "]";
			worldViewShader->setUniform(ss.str(), static_cast<glm::vec2>(*it2));
			i++;
		}
		worldViewShader->setUniform("numClipVerticies", i);
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*worldViewShader, model);
		}
		worldViewShader->finish();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
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
