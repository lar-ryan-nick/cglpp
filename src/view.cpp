#include "view.h"

View::View() : shader("res/glsl/viewVertexShader.glsl", "res/glsl/viewFragmentShader.glsl"), backgroundColor(1.0f, 0.0f, 0.0f, 1.0f) {
	std::cout << "Hello" << std::endl;
}

void View::draw() {
	std::cout << "Hello" << std::endl;
	shader.use();
	shader.setUniform("model", 1, false, glm::value_ptr(model));
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 0.0f);
	shader.setUniform("projection", 1, false, glm::value_ptr(projection));
	shader.setUniform("backgroundColor", backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	square.bindVAO();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
