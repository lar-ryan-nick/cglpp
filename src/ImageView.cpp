#include "ImageView.h"

cgl::Shader* cgl::ImageView::imageViewShader = NULL;

cgl::ImageView::ImageView(const std::string& textureFilename, float x, float y, float width, float height) : View(x, y, width, height), texture(textureFilename) {
	if (imageViewShader == NULL) {
		imageViewShader = new Shader("res/glsl/ImageViewVertexShader.glsl", "res/glsl/ImageViewFragmentShader.glsl");
	}
	shader = imageViewShader;
}

cgl::ImageView::ImageView(const Texture& t, float x, float y, float width, float height) : View(x, y, width, height), texture(t) {
	if (imageViewShader == NULL) {
		imageViewShader = new Shader("res/glsl/ImageViewVertexShader.glsl", "res/glsl/ImageViewFragmentShader.glsl");
	}
	shader = imageViewShader;
}

void cgl::ImageView::setTexture(const Texture& t) {
	texture = t;
}

void cgl::ImageView::setTexture(const std::string& textureFilename) {
	texture = Texture(textureFilename);
}

void cgl::ImageView::draw(const glm::mat4& parentModel, const Polygon& p) {
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	shader->setUniform("image", 0);
	View::draw(parentModel, p);
}
