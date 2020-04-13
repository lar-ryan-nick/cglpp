#include "WorldView.h"
#include "ImageView.h"

cgl::Shader* cgl::WorldView::worldViewShader = NULL;
cgl::Shader* cgl::WorldView::shadowMapShader = NULL;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height), pitch(0.0f), yaw(-90.0f) {
	if (worldViewShader == NULL) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl");
	}
	if (shadowMapShader == NULL) {
		shadowMapShader = new Shader("res/glsl/ShadowMapVertexShader.glsl", "res/glsl/ShadowMapFragmentShader.glsl");
	}
	camera.setRotation(pitch, yaw);
}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& poly) {
	View::draw(parentModel, poly);
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
	glm::vec3 lightDirection(4.0f, -3.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);
	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::mat4 m(1.0f);
	glm::mat4 view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 300.0f);
	glm::mat4 vp = projection * view;

	shadowMap.updateSplits(1.0f, 300.0f);
	shadowMap.updateLightViewProjections(camera, directionalLight, glm::radians(45.0f), viewport[2] / viewport[3]);
	glEnable(GL_DEPTH_TEST);
	shadowMapShader->use();
	for (int i = 0; i < shadowMap.getNumCascades(); ++i) {
		// Render to shadowMap
		//glCullFace(GL_FRONT);
		shadowMap.bindFramebuffer(i);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowMapShader->setUniform("lightVP", shadowMap.getLightViewProjection(i));
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*shadowMapShader, m);
		}
	}
	shadowMapShader->finish();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		std::list<Position> vert = it1->getVerticies();
		if (vert.size() < 3) {
			continue;
		}

		//glCullFace(GL_BACK);
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
		int i;
		for (i = 0; i < shadowMap.getNumCascades(); ++i) {
			worldViewShader->setUniform("lightVP[" + std::to_string(i) + "]", shadowMap.getLightViewProjection(i));
		}
		for (; i < shadowMap.MAX_CASCADES; ++i) {
			worldViewShader->setUniform("lightVP[" + std::to_string(i) + "]", glm::mat4(1.0f));
		}
		for (i = 0; i < shadowMap.getNumCascades(); ++i) {
			glm::vec4 projectedCascadeEnd = projection * glm::vec4(0.0, 0.0f, -shadowMap.getCascadeEnd(i), 1.0f);
			worldViewShader->setUniform("cascadeEnds[" + std::to_string(i) + "]", projectedCascadeEnd.z);
		}
		for (; i < shadowMap.MAX_CASCADES; ++i) {
			worldViewShader->setUniform("cascadeEnds[" + std::to_string(i) + "]", std::numeric_limits<float>::lowest());
		}
		glActiveTexture(GL_TEXTURE0 + 8);
		shadowMap.bindShadowMapArray();
		worldViewShader->setUniform("cascadedShadowMap", 8);
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*worldViewShader, m);
		}
		worldViewShader->finish();
		for (int i = 0; i < clipPlaneCount; i++) {
			glDisable(GL_CLIP_DISTANCE0 + i);
		}
	}
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
