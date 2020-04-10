#include "WorldView.h"
#include <limits>
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
	glm::vec3 lightDirection(4.0f, -3.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);
	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::mat4 m(1.0f);
	/*
	m = glm::translate(m, glm::vec3(30.0f, 0.0f, 30.0f));
	m = glm::scale(m, glm::vec3(0.2f, 0.2f, 0.2f));
	*/
	glm::mat4 view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 100.0f);
	glm::mat4 vp = projection * view;

	glm::mat4 viewInv = glm::inverse(view);
	float minX, maxX, minY, maxY, minZ, maxZ;
	minX = minY = minZ = std::numeric_limits<float>::max();
	maxX = maxY = maxZ = std::numeric_limits<float>::lowest();
	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 0.0f,  0.0f), directionalLight.getDirection(), glm::vec3(0.0f, 1.0f,  0.0f));
	float tanHalfHFOV = glm::tan(glm::radians(45.0f / 2.0f));
	float tanHalfVFOV = glm::tan(glm::radians(45.0f * viewport[2] / viewport[3] / 2.0f));
	float zn = 1.0f;
	float zf = 40.0f;
	float xn = zn * tanHalfHFOV;
	float xf = zf * tanHalfHFOV;
	float yn = zn * tanHalfVFOV;
	float yf = zf * tanHalfVFOV;
	glm::vec4 frustumCorners[8] = {
		glm::vec4(xn, yn, -zn, 1.0f),
		glm::vec4(-xn, yn, -zn, 1.0f),
		glm::vec4(xn, -yn, -zn, 1.0f),
		glm::vec4(-xn, -yn, -zn, 1.0f),
		glm::vec4(xf, yf, -zf, 1.0f),
		glm::vec4(-xf, yf, -zf, 1.0f),
		glm::vec4(xf, -yf, -zf, 1.0f),
		glm::vec4(-xf, -yf, zf, 1.0f)
	};
	std::cout << "Camera Position: " << Position(camera.getPosition()) << std::endl;
	std::cout << "Camera Direction: " << Position(camera.getDirection()) << std::endl;
	std::cout << "Frustum Corners:" << std::endl;
	for (int i = 0; i < 8; ++i) {
		glm::vec4 lightFrustumCorner = viewInv * frustumCorners[i];
		std::cout << Position(lightFrustumCorner) << std::endl;
		lightFrustumCorner = lightView * lightFrustumCorner;
		minX = std::min(minX, lightFrustumCorner.x);
		maxX = std::max(maxX, lightFrustumCorner.x);
		minY = std::min(minY, lightFrustumCorner.y);
		maxY = std::max(maxY, lightFrustumCorner.y);
		minZ = std::min(minZ, lightFrustumCorner.z);
		maxZ = std::max(maxZ, lightFrustumCorner.z);
	}

	std::cout << "Min x: " << minX << std::endl;
	std::cout << "Max x: " << maxX << std::endl;
	std::cout << "Min y: " << minY << std::endl;
	std::cout << "Max y: " << maxY << std::endl;
	std::cout << "Min z: " << minZ << std::endl;
	std::cout << "Max z: " << maxZ << std::endl;
	
	glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	//glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
	glm::mat4 lightVP = lightProjection * lightView;
	//std::cout << Position(lightVP * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) << std::endl;

	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		std::list<Position> vert = it1->getVerticies();
		if (vert.size() < 3) {
			continue;
		}
		glEnable(GL_DEPTH_TEST);

		// Render to shadowMap
		glCullFace(GL_FRONT);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowMap.bindFramebuffer();
		shadowMapShader->use();
		shadowMapShader->setUniform("lightVP", lightVP);
		shadowMapShader->setUniform("model", m);
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*shadowMapShader, m);
		}
		shadowMapShader->finish();

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
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

		worldViewShader->setUniform("lightVP", lightVP);
		glActiveTexture(GL_TEXTURE0 + 8);
		shadowMap.bindTexture();
		worldViewShader->setUniform("shadowMap", 8);

		worldViewShader->setUniform("model", m);
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*worldViewShader, m);
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
