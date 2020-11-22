#include "ImageView.h"

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

void cgl::ImageView::render(const Polygon& bounds, const glm::mat4& model) {
	View::render(bounds, model);
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f, -0.1f, 0.1f);
	glm::mat4 mvp = projection * model;
	Rectangle bnds = getBounds();
	float verticies[8];
	verticies[0] = verticies[6] = bnds.getX();
	verticies[1] = verticies[3] = bnds.getY();
	verticies[2] = verticies[4] = bnds.getX() + bnds.getWidth();
	verticies[5] = verticies[7] = bnds.getY() + bnds.getHeight();
	for (int i = 0; i < 8; i += 2) {
			glm::vec4 ver(verticies[i], verticies[i + 1], 0.0f, 1.0f);
			glm::vec4 transformed = mvp * ver;
			verticies[i] = transformed.x;
			verticies[i + 1] = transformed.y;
	}
	float a = (1 + (verticies[1] - verticies[5]) / (verticies[3] - verticies[1])) / (verticies[4] - verticies[0] + (verticies[5] - verticies[1]) * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]));
	float b = (1 + a * (verticies[0] - verticies[2])) / (verticies[3] - verticies[1]);
	float d = -a * verticies[0] - b * verticies[1];
	float e = -1 / (verticies[4] - verticies[0] + (verticies[5] - verticies[1]) * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]));
	float f = e * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]);
	float h = 1 - e * verticies[0] - f * verticies[1];
	glm::mat4 textureMapper(a, e, 0.0f, 0.0f, b, f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, d, h, 0.0f, 0.0f);
	std::list<Position> vert = bounds.getVerticies();
	glBindVertexArray(vao);
	imageViewShader->use();
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	imageViewShader->setUniform("image", 0);
	imageViewShader->setUniform("textureMapper", textureMapper);
	glDrawElements(GL_TRIANGLES, (vert.size() - 2) * 3, GL_UNSIGNED_INT, 0);
	imageViewShader->finish();
	glBindVertexArray(0);
}
