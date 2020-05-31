#include "Pipeline.h"
#include <glad/glad.h>

cgl::Pipeline::Pipeline(const Size& size) {
	// Create texture for the framebuffer to draw into
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.getWidth(), size.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create framebuffer and bind to texture
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer error status: " << status << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

cgl::Pipeline::~Pipeline() {
	glDeleteTextures(1, &texture);
	glDeleteFramebuffers(1, &fbo);
}

void cgl::Pipeline::bindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void cgl::Pipeline::bindTexture() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void cgl::Pipeline::updateSize(const Size& size) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.getWidth(), size.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}
