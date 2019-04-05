#include "../include/ImageView.h"

cgl::Shader* cgl::ImageView::imageViewShader = NULL;

cgl::ImageView::ImageView(const std::string& textureFilename, float x, float y, float width, float height) : View(x, y, width, height), texture(textureFilename) {
	if (imageViewShader == NULL) {
		imageViewShader = new Shader("res/glsl/ImageViewVertexShader.glsl", "res/glsl/ImageViewFragmentShader.glsl");
	}
}

cgl::ImageView::ImageView(const Texture& t, float x, float y, float width, float height) : View(x, y, width, height), texture(t) {
	if (imageViewShader == NULL) {
		imageViewShader = new Shader("res/glsl/ImageViewVertexShader.glsl", "res/glsl/ImageViewFragmentShader.glsl");
	}
}

void cgl::ImageView::setTexture(const Texture& t) {
	texture = t;
}

void cgl::ImageView::setTexture(const std::string& textureFilename) {
	texture = Texture(textureFilename);
}

void cgl::ImageView::render(const Polygon& bounds, const glm::mat4& textureMapper) {
	std::list<Position> vert = bounds.getVerticies();
	std::vector<glm::vec2> v(vert.begin(), vert.end());
	View::render(bounds, textureMapper);
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	glBindVertexArray(vao);
	imageViewShader->use();
	imageViewShader->setUniform("image", 0);
	imageViewShader->setUniform("textureMapper", textureMapper);
	glDrawElements(GL_TRIANGLES, (vert.size() - 2) * 3, GL_UNSIGNED_INT, 0);
	imageViewShader->finish();
	glBindVertexArray(0);
}
