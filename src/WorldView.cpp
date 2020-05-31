#include "WorldView.h"

cgl::Shader* cgl::WorldView::gBufferShader = nullptr;
cgl::Shader* cgl::WorldView::worldViewShader = nullptr;
cgl::Shader* cgl::WorldView::shadowMapShader = nullptr;
cgl::Shader* cgl::WorldView::reflectionShader = nullptr;
cgl::Shader* cgl::WorldView::canvasShader = nullptr;

cgl::WorldView::WorldView(float x, float y, float width, float height) : View(x, y, width, height), pitch(0.0f), yaw(-90.0f), shadowMap(3, 1 << 12) {
	if (gBufferShader == nullptr) {
		gBufferShader = new Shader("res/glsl/GBufferVertexShader.glsl", "res/glsl/GBufferFragmentShader.glsl");
	}
	if (worldViewShader == nullptr) {
		worldViewShader = new Shader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldFragmentShader.glsl");
	}
	if (shadowMapShader == nullptr) {
		shadowMapShader = new Shader("res/glsl/ShadowMapVertexShader.glsl", "res/glsl/ShadowMapFragmentShader.glsl");
	}
	if (reflectionShader == nullptr) {
		reflectionShader = new Shader("res/glsl/ReflectionVertexShader.glsl", "res/glsl/ReflectionFragmentShader.glsl");
	}
	if (canvasShader == nullptr) {
		canvasShader = new Shader("res/glsl/CanvasVertexShader.glsl", "res/glsl/CanvasFragmentShader.glsl");
	}
	camera.setRotation(pitch, yaw);
}

void cgl::WorldView::addActor(Actor* a) {
	actors.push_back(a);
}

void cgl::WorldView::draw(const glm::mat4& parentModel, const Polygon& poly) {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	geometryPass();
	shadowPass();

	glDisable(GL_DEPTH_TEST);

	screenSpacePass();
	lightingPass();
	postProcessPass();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_BLEND);

	// Lighting Shader use is done below
	View::draw(parentModel, poly);
}

void cgl::WorldView::geometryPass() {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::vec3 lightDirection(4.0f, -6.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);

	glm::mat4 m(1.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 300.0f);
	glm::mat4 vp = projection * view;

	// GBuffer pass
	gBuffer.bindBuffer();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_BACK);
	gBufferShader->use();
	gBufferShader->setUniform("vp", vp);
	for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
		Actor* actor = *it2;
		actor->draw(*gBufferShader, m);
	}
	gBufferShader->finish();
}

void cgl::WorldView::shadowPass() {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::vec3 lightDirection(4.0f, -6.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);

	glm::mat4 m(1.0f);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	// Shadow map pass
	shadowMap.updateSplits(1.0f, 300.0f);
	shadowMap.updateLightViewProjections(camera, directionalLight, glm::radians(45.0f), viewport[2] / viewport[3]);
	shadowMapShader->use();
	for (int i = 0; i < shadowMap.getNumCascades(); ++i) {
		// Render to shadowMap
		shadowMap.bindFramebuffer(i);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowMapShader->setUniform("lightVP", shadowMap.getLightViewProjection(i));
		for (std::list<Actor*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
			Actor* actor = *it2;
			actor->draw(*shadowMapShader, m);
		}
	}
	shadowMapShader->finish();

	//glCullFace(GL_FRONT);
	//glDisable(GL_CULL_FACE);
	// Reset viewport
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void cgl::WorldView::screenSpacePass() {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 300.0f);

	// SSAO pass
	ssao.draw(gBuffer, view, projection);

	// SSR pass
	ssr.draw(gBuffer, view, projection);
}

void cgl::WorldView::lightingPass() {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	//SpotLight spotLight(camera.getDirection(), camera.getPosition());
	glm::vec3 lightDirection(4.0f, -6.0f, -4.0f);
	DirectionalLight directionalLight(lightDirection);

	glm::mat4 m(1.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 1.0f, 300.0f);
	glm::mat4 vp = projection * view;

	lightingPipeline.updateSize(Size(viewport[2], viewport[3]));
	lightingPipeline.bindFramebuffer();
	worldViewShader->use();
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
	shadowMap.bindShadowMapArray();
	worldViewShader->setUniform("cascadedShadowMap", 2);
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
	ssao.bindSSAO();
	worldViewShader->setUniform("ssao", 6);
	VAO::getScreenVAO()->draw();
	worldViewShader->finish();
}

void cgl::WorldView::postProcessPass() {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	postProcessPipeline.updateSize(Size(viewport[2], viewport[3]));

	postProcessPipeline.bindFramebuffer();
	reflectionShader->use();
	glActiveTexture(GL_TEXTURE0 + 0);
	lightingPipeline.bindTexture();
	reflectionShader->setUniform("scene", 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	ssr.bindSSR();
	reflectionShader->setUniform("ssr", 1);
	glActiveTexture(GL_TEXTURE0 + 2);
	gBuffer.bindSpecularTexture();
	reflectionShader->setUniform("gSpecular", 2);
	VAO::getScreenVAO()->draw();
	reflectionShader->finish();
}


void cgl::WorldView::render(const Polygon& bounds, const glm::mat4& model) {
	View::render(bounds, model);

	std::list<Position> vert = bounds.getVerticies();

	canvasShader->use();
	glActiveTexture(GL_TEXTURE0 + 0);
	postProcessPipeline.bindTexture();
	canvasShader->setUniform("image", 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, (vert.size() - 2) * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	canvasShader->finish();
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
