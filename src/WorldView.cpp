#include "WorldView.h"

cgl::Shader* cgl::WorldView::gBufferShader = nullptr;
cgl::Shader* cgl::WorldView::worldViewShader = nullptr;
cgl::Shader* cgl::WorldView::shadowMapShader = nullptr;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height), pitch(0.0f), yaw(-90.0f) {
	if (gBufferShader == nullptr) {
		gBufferShader = new Shader("res/glsl/GBufferVertexShader.glsl", "res/glsl/GBufferFragmentShader.glsl");
	}
	if (worldViewShader == nullptr) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl");
	}
	if (shadowMapShader == nullptr) {
		shadowMapShader = new Shader("res/glsl/ShadowMapVertexShader.glsl", "res/glsl/ShadowMapFragmentShader.glsl");
	}
	camera.setRotation(pitch, yaw);
}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& poly) {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::vec3 lightDirection(4.0f, -3.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);

	glm::mat4 m(1.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 300.0f);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	shadowMap.updateSplits(1.0f, 300.0f);
	shadowMap.updateLightViewProjections(camera, directionalLight, glm::radians(45.0f), viewport[2] / viewport[3]);
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
	// Reset viewport
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	gBuffer.bindBuffer();
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_BACK);
	gBufferShader->use();
	gBufferShader->setUniform("vp", projection * view);
	for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
		Actor* actor = *it2;
		actor->draw(*gBufferShader, m);
	}
	gBufferShader->finish();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	View::draw(parentModel, poly);
}

void cgl::WorldView::render(const Polygon& bounds, const glm::mat4& model) {
	View::render(bounds, model);

	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	/*
	// TODO: fix texture mapper math to prevent divide by zero
	glm::mat4 proj = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f, -0.1f, 0.1f);
	glm::mat4 mvp = proj * model;
	Rectangle bnds = getBounds();
	float verticies[8];
	verticies[0] = verticies[6] = bnds.getX();
	verticies[1] = verticies[3] = bnds.getY();
	verticies[2] = verticies[4] = bnds.getX() + bnds.getWidth();
	verticies[5] = verticies[7] = bnds.getY() + bnds.getHeight();
	for (int i = 0; i < 8; i += 2) {
			glm::vec4 ver(verticies[i], verticies[i + 1], 0.0f, 1.0f);
			glm::vec4 transformed = mvp * ver;
			verticies[i] = transformed.x;
			verticies[i + 1] = transformed.y;
			//std::cout << "<" << transformed.x << ", " << transformed.y << ">" << std::endl;
	}
	float a = (1 + (verticies[1] - verticies[5]) / (verticies[3] - verticies[1])) / (verticies[4] - verticies[0] + (verticies[5] - verticies[1]) * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]));
	float b = (1 + a * (verticies[0] - verticies[2])) / (verticies[3] - verticies[1]);
	float d = -a * verticies[0] - b * verticies[1];
	float e = -1 / (verticies[4] - verticies[0] + (verticies[5] - verticies[1]) * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]));
	float f = e * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]);
	float h = 1 - e * verticies[0] - f * verticies[1];
	glm::mat4 textureMapper(a, e, 0.0f, 0.0f, b, f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, d, h, 0.0f, 0.0f);

	//std::cout << a << ' ' << b << ' ' << d << ' ' << e << std::endl;
	//std::cout << VectorContainer(textureMapper * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f)) << std::endl;
	//std::cout << VectorContainer(textureMapper * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f)) << std::endl;
	//std::cout << VectorContainer(textureMapper * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)) << std::endl;
	//std::cout << VectorContainer(textureMapper * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)) << std::endl;
	*/

	std::list<Position> vert = bounds.getVerticies();


	glm::vec3 lightDirection(4.0f, -3.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);
	glm::mat4 m(1.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 300.0f);

	worldViewShader->use();
	//worldViewShader->setUniform("textureMapper", textureMapper);
	worldViewShader->setUniform("vp", projection * view);
	worldViewShader->setUniform("viewPosition", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	worldViewShader->setUniform("directionalLight", directionalLight);
	//worldViewShader->setUniform("spotLight", spotLight);
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
	glActiveTexture(GL_TEXTURE0 + 0);
	gBuffer.bindPositionTexture();
	worldViewShader->setUniform("gPosition", 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	gBuffer.bindNormalTexture();
	worldViewShader->setUniform("gNormal", 1);
	glActiveTexture(GL_TEXTURE0 + 2);
	gBuffer.bindTexCoordTexture();
	worldViewShader->setUniform("gTexCoord", 2);
	glActiveTexture(GL_TEXTURE0 + 3);
	gBuffer.bindAmbientTexture();
	worldViewShader->setUniform("gAmbient", 3);
	glActiveTexture(GL_TEXTURE0 + 4);
	gBuffer.bindDiffuseTexture();
	worldViewShader->setUniform("gDiffuse", 4);
	glActiveTexture(GL_TEXTURE0 + 5);
	gBuffer.bindSpecularTexture();
	worldViewShader->setUniform("gSpecular", 5);
	glActiveTexture(GL_TEXTURE0 + 6);
	shadowMap.bindShadowMapArray();
	worldViewShader->setUniform("cascadedShadowMap", 6);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, (vert.size() - 2) * 3, GL_UNSIGNED_INT, 0);
	worldViewShader->finish();
	glBindVertexArray(0);
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
