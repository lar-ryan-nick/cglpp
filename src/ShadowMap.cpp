#include "ShadowMap.h"

cgl::ShadowMap::ShadowMap(unsigned int s) : size(s) {
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int mapID;
	glGenTextures(1, &mapID);
	depthMap.setID(mapID);
	depthMap.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.getID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

cgl::ShadowMap::~ShadowMap() {
	unsigned int textureID = depthMap.getID();
	glDeleteTextures(1, &textureID);
	glDeleteFramebuffers(1, &depthMapFBO);
}

void cgl::ShadowMap::bindFramebuffer() {
	glViewport(0, 0, size, size);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
}

void cgl::ShadowMap::unbindFramebuffer() {
	// Have a way to reset the viewport here?
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cgl::ShadowMap::bindTexture() {
	depthMap.bind();
}
