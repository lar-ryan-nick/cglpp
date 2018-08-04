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
	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		Polygon clippedPolygon = *it1;
		std::list<Position> vert = clippedPolygon.getVerticies();
		if (vert.size() < 3) {
			continue;
		}
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 view = camera.getViewMatrix();
		float viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 100.0f);
		glm::mat4 mvp = projection * view * parentModel * model;
		cgl::SpotLight spotLight;
		spotLight.setPosition(camera.getPosition());
		spotLight.setDirection(camera.getDirection());
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		worldViewShader->use();
		worldViewShader->setUniform("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		worldViewShader->setUniform("light", spotLight);
		worldViewShader->setUniform("vp", projection * view);
		worldViewShader->setUniform("model", model);
		int i = 0;
		for (std::list<Position>::iterator it = vert.begin(); it != vert.end(); it++) {
			std::stringstream ss;
			ss << "clipRegion[" << i++ << "]";
			worldViewShader->setUniform(ss.str(), static_cast<glm::vec2>(*it));
		}
		worldViewShader->setUniform("numClipVerticies", i);
		for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
			Actor* actor = *it;
			actor->draw(*worldViewShader, model);
		}
		worldViewShader->finish();
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
