#include "imageView.h"

Shader* ImageView::imageViewShader = NULL;

ImageView::ImageView(const std::string& textureFilename, float x, float y, float width, float height) : View(x, y, width, height), texture(textureFilename) {
	if (imageViewShader == NULL) {
		imageViewShader = new Shader("res/glsl/imageViewVertexShader.glsl", "res/glsl/imageViewFragmentShader.glsl");
	}
	shader = imageViewShader;
}

ImageView::ImageView(const Texture& t, float x, float y, float width, float height) : View(x, y, width, height), texture(t) {
	if (imageViewShader == NULL) {
		imageViewShader = new Shader("res/glsl/imageViewVertexShader.glsl", "res/glsl/imageViewFragmentShader.glsl");
	}
	shader = imageViewShader;
}

void ImageView::setTexture(const Texture& t) {
	texture = t;
}

void ImageView::setTexture(const std::string& textureFilename) {
	texture = Texture(textureFilename);
}

void ImageView::draw(float parentX, float parentY, float parentWidth, float parentHeight) {
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	shader->setUniform("image", 0);
	View::draw(parentX, parentY, parentWidth, parentHeight);
}
