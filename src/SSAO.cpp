#include "SSAO.h"

cgl::Shader* cgl::SSAO::ssaoShader = nullptr;
cgl::Shader* cgl::SSAO::bilateralFilterShader = nullptr;
unsigned int cgl::SSAO::vao = 0;
unsigned int cgl::SSAO::vbo = 0;
unsigned int cgl::SSAO::ebo = 0;
std::vector<float> cgl::SSAO::bilateralFilterKernel;

float cgl::SSAO::normpdf(float x) {
	return 0.39894 * glm::exp(-0.5f * x * x / (SSIGMA * SSIGMA)) / SSIGMA;
}

cgl::SSAO::SSAO() {
	if (ssaoShader == nullptr) {
		ssaoShader = new Shader("res/glsl/SSAOVertexShader.glsl", "res/glsl/SSAOFragmentShader.glsl");
	}
	if (bilateralFilterShader == nullptr) {
		bilateralFilterShader = new Shader("res/glsl/SSAOVertexShader.glsl", "res/glsl/BilateralFilterFragmentShader.glsl");
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
	if (bilateralFilterKernel.empty()) {
		bilateralFilterKernel.resize(2 * BFK_SIZE + 1);
		for (int i = 0; i < BFK_SIZE; ++i) {
			bilateralFilterKernel[BFK_SIZE - i] = bilateralFilterKernel[BFK_SIZE + i] = normpdf(i);
		}
	}
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
	std::default_random_engine generator;
	// TODO: replace 64 with KERNEL_SIZE constexpr
	for (unsigned int i = 0; i < 64; ++i) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample  = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 64.0; 
		scale = glm::mix(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);  
	}
	// TODO: replace 16 with NOISE_SIZE * NOISE_SIZE
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); 
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glGenTextures(1, &ssaoBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, viewport[2], viewport[3], 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBuffer, 0);

	glGenFramebuffers(1, &bilateralFilterFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bilateralFilterFBO);
	glGenTextures(1, &bilateralFilterBuffer);
	glBindTexture(GL_TEXTURE_2D, bilateralFilterBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, viewport[2], viewport[3], 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bilateralFilterBuffer, 0);
}

void cgl::SSAO::draw(GBuffer& gBuffer, const glm::mat4& view, const glm::mat4& projection) {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoShader->use();
	glActiveTexture(GL_TEXTURE0 + 0);
	gBuffer.bindPositionTexture();
	ssaoShader->setUniform("gPosition", 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	gBuffer.bindNormalTexture();
	ssaoShader->setUniform("gNormal", 1);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	ssaoShader->setUniform("noise", 2);
	ssaoShader->setUniform("view", view);
	ssaoShader->setUniform("projection", projection);
	ssaoShader->setUniform("kernel", ssaoKernel);
	ssaoShader->setUniform("screenWidth", viewport[2]);
	ssaoShader->setUniform("screenHeight", viewport[3]);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	ssaoShader->finish();

	glBindFramebuffer(GL_FRAMEBUFFER, bilateralFilterFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	bilateralFilterShader->use();
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, ssaoBuffer);
	ssaoShader->setUniform("image", 0);
	ssaoShader->setUniform("kernel", bilateralFilterKernel);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	bilateralFilterShader->finish();
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cgl::SSAO::bindSSAO() {
	glBindTexture(GL_TEXTURE_2D, bilateralFilterBuffer);
}
