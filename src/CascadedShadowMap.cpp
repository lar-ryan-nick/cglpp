#include "CascadedShadowMap.h"

cgl::CascadedShadowMap::CascadedShadowMap(int nc, unsigned int size) : shadowMapSize(size), numCascades[nc], shadowMaps(new Texture[numCascades]) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	unsigned int* textureIDs = new unsigned int[numCascades];
	glGenTextures(numCascades, textureIDs);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	for (int i = 0; i < numCascades; ++i) {
		shadowMaps[i].setID(textureIDs[i]);
		shadowMaps[i].bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
}

cgl::CascadedShadowMap::~CascadedShaowMap() {
	glDeleteFramebuffers(1, &depthMapFBO);
	unsigned int* textureIDs = new unsigned int[numCascades];
	for (int i = 0; i < numCascades; ++i) {
		textureIDs[i] = shadowMaps[i].getID();
	}
	glDeleteTextures(numCascades, textureIDs);
	delete[] textureIDs;
	delete[] shadowMaps;
}

void cgl::CascadedShadowMap::bindFramebuffer(int cascadeNum) {
	glViewport(0, 0, shadowMapSize, shadowMapSize);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps[cascadeNum].getID(), 0);
}

cgl::Texture& cgl::CascadedShadowMap::getShadowMap(int cascadeNum) {
	return shadowMaps[cascadeNum];
}
