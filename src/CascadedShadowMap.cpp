#include "CascadedShadowMap.h"

cgl::CascadedShadowMap::CascadedShadowMap(int nc, unsigned int size) : shadowMapSize(size), numCascades(nc) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenTextures(1, &shadowMapArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapArray);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, MAX_CASCADES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
}

cgl::CascadedShadowMap::~CascadedShadowMap() {
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &shadowMapArray);
}

int cgl::CascadedShadowMap::getNumCascades() const {
	return numCascades;
}

void cgl::CascadedShadowMap::bindFramebuffer(int cascadeNum) {
	glViewport(0, 0, shadowMapSize, shadowMapSize);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapArray, 0, cascadeNum);
}

void cgl::CascadedShadowMap::bindShadowMapArray() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapArray);
}

float cgl::CascadedShadowMap::getCascadeEnd(int cascadeNum) const {
	return cascadeSplits[cascadeNum + 1];
}

glm::mat4 cgl::CascadedShadowMap::getLightViewProjection(int cascadeNum) const {
	return lightViewProjections[cascadeNum];
}

void cgl::CascadedShadowMap::updateSplits(float zNear, float zFar) {
	float lambda = 0.5f;
	float ratio = zFar / zNear;
	cascadeSplits[0] = zNear;
	for (int i = 1; i < numCascades; ++i) {
		float weight = static_cast<float>(i) / numCascades;
		cascadeSplits[i] = lambda * zNear * glm::pow(ratio, weight) + (1 - lambda) * (zNear + weight * zFar - zNear);
	}
	cascadeSplits[numCascades] = zFar;
}

void cgl::CascadedShadowMap::updateLightViewProjections(const Camera& camera, const DirectionalLight& directionalLight, float yfov, float aspectRatio) {
	glm::mat4 viewInv = glm::inverse(camera.getViewMatrix());
	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 0.0f,  0.0f), directionalLight.getDirection(), glm::vec3(0.0f, 1.0f,  0.0f));
	float minX, maxX, minY, maxY, minZ, maxZ;
	float tanHalfVFOV = glm::tan(yfov / 2.0f);
	float tanHalfHFOV = glm::tan(yfov * aspectRatio / 2.0f);
	for (int i = 0; i < numCascades; ++i) {
		minX = minY = minZ = std::numeric_limits<float>::max();
		maxX = maxY = maxZ = std::numeric_limits<float>::lowest();
		float zn = cascadeSplits[i];
		float zf = cascadeSplits[i + 1];
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
			glm::vec4(-xf, -yf, -zf, 1.0f)
		};
		for (int i = 0; i < 8; ++i) {
			glm::vec4 lightFrustumCorner = lightView * viewInv * frustumCorners[i];
			minX = glm::min(minX, lightFrustumCorner.x);
			maxX = glm::max(maxX, lightFrustumCorner.x);
			minY = glm::min(minY, lightFrustumCorner.y);
			maxY = glm::max(maxY, lightFrustumCorner.y);
			minZ = glm::min(minZ, lightFrustumCorner.z);
			maxZ = glm::max(maxZ, lightFrustumCorner.z);
		}
		//minZ -= 30.0f;
		maxZ += 40.0f;
		glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
		lightViewProjections[i] = lightProjection * lightView;
	}
}
