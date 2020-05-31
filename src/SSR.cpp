#include "SSR.h"

cgl::Shader* cgl::SSR::ssrShader = nullptr;
unsigned int cgl::SSR::vao = 0;
unsigned int cgl::SSR::vbo = 0;
unsigned int cgl::SSR::ebo = 0;

cgl::SSR::SSR() {
	if (ssrShader == nullptr) {
		ssrShader = new Shader("res/glsl/SSRVertexShader.glsl", "res/glsl/SSRFragmentShader.glsl");
	}
	if (vao == 0) {
		// Init framebuffer vao
		float verticies[] = {
			-1.0f, -1.0f,
			1.0f, -1.0f,
			1.0f, 1.0f,
			-1.0f, 1.0f
		};
		unsigned int indicies[] = {
			0, 1, 3,
			1, 2, 3
		};
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	glGenFramebuffers(1, &ssrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssrFBO);
	glGenTextures(1, &ssrBuffer);
	glBindTexture(GL_TEXTURE_2D, ssrBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, viewport[2], viewport[3], 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssrBuffer, 0);
}

void cgl::SSR::draw(GBuffer& gBuffer, const glm::mat4& view, const glm::mat4& projection) {
	glBindFramebuffer(GL_FRAMEBUFFER, ssrFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ssrShader->use();
	glActiveTexture(GL_TEXTURE0 + 0);
	gBuffer.bindPositionTexture();
	ssrShader->setUniform("gPosition", 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	gBuffer.bindNormalTexture();
	ssrShader->setUniform("gNormal", 1);
	/*
	glActiveTexture(GL_TEXTURE0 + 3);
	gBuffer.bindBitangentTexture();
	worldViewShader->setUniform("gBitangent", 3);
	*/
	ssrShader->setUniform("view", view);
	ssrShader->setUniform("projection", projection);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	ssrShader->finish();
}

void cgl::SSR::bindSSR() {
	glBindTexture(GL_TEXTURE_2D, ssrBuffer);
}
