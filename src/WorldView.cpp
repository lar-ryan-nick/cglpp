#include "../include/WorldView.h"

cgl::Shader* cgl::WorldView::worldViewShader = NULL;
float cgl::WorldView::pitch = 0.0f;
float cgl::WorldView::yaw = 90.0f;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height), camera(glm::vec3(0.0f, 0.0f, -3.0f)) {
	if (worldViewShader == NULL) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl");
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
	DirectionalLight directionalLight(glm::vec3(.5f, .5f, -2.0f));
	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::mat4 m(1.0f);
	/*
	m = glm::translate(m, glm::vec3(0.0f, 0.0f, -1.75f));
	m = glm::scale(m, glm::vec3(0.2f, 0.2f, 0.2f));
	*/
	glm::mat4 view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 1000000000.0f);
	glm::mat4 vp = projection * view;
	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		std::list<Position> vert = it1->getVerticies();
		if (vert.size() < 3) {
			continue;
		}
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		worldViewShader->use();
		// set clip planes
		int clipPlaneCount = 0;
		std::list<Position>::iterator vertIt1 = vert.begin();
		for (std::list<Position>::iterator vertIt2 = std::next(vertIt1); vertIt2 != vert.end(); vertIt2++) {
			glm::vec3 line = glm::cross(glm::vec3(vertIt2->getX(), vertIt2->getY(), vertIt2->getW()), glm::vec3(vertIt1->getX(), vertIt1->getY(), vertIt1->getW()));
			glm::vec4 plane(line.x, line.y, 0.0, line.z);
			glEnable(GL_CLIP_DISTANCE0 + clipPlaneCount);
			worldViewShader->setUniform(("clipPlane[" + std::to_string(clipPlaneCount)) + ']', plane);
			vertIt1++;
			clipPlaneCount++;
		}
		glEnable(GL_CLIP_DISTANCE0 + clipPlaneCount);
		std::list<Position>::iterator vertIt2 = vert.begin();
		glm::vec3 line = glm::cross(glm::vec3(vertIt2->getX(), vertIt2->getY(), vertIt2->getW()), glm::vec3(vertIt1->getX(), vertIt1->getY(), vertIt1->getW()));
		glm::vec4 plane(line.x, line.y, 0.0, line.z);
		worldViewShader->setUniform(("clipPlane[" + std::to_string(clipPlaneCount)) + ']', plane);
		clipPlaneCount++;
		worldViewShader->setUniform("numPlanes", clipPlaneCount);
		worldViewShader->setUniform("viewPosition", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		worldViewShader->setUniform("directionalLight", directionalLight);
		//worldViewShader->setUniform("spotLight", spotLight);
		worldViewShader->setUniform("vp", vp);
		worldViewShader->setUniform("model", m);
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*worldViewShader, model);
		}
		worldViewShader->finish();
		for (int i = 0; i < clipPlaneCount; i++) {
			glDisable(GL_CLIP_DISTANCE0 + i);
		}
		glDisable(GL_DEPTH_TEST);
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
