#include "view.h"

View::View(float x, float y, float width, float height) : shader("res/glsl/viewVertexShader.glsl", "res/glsl/viewFragmentShader.glsl"), backgroundColor(1.0f, 0.0f, 0.0f, 1.0f) {
	verticies[0] = verticies[6] = x;
	verticies[1] = verticies[3] = y;
	verticies[2] = verticies[4] = x + width;
	verticies[5] = verticies[7] = y + height;
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void View::setBounds(float x, float y, float width, float height) {
	verticies[0] = verticies[6] = x;
	verticies[1] = verticies[3] = y;
	verticies[2] = verticies[4] = x + width;
	verticies[5] = verticies[7] = y + height;
}

Rectangle View::getBounds() const {
	return Rectangle(verticies[0], verticies[1], verticies[2] - verticies[0], verticies[5] - verticies[1]);
}

void View::setBackgroundColor(const Color& bc) {
	backgroundColor = bc;
}

void View::draw(float parentX, float parentY, float parentWidth, float parentHeight) {
	shader.use();
	shader.setUniform("model", 1, false, glm::value_ptr(model));
	glm::mat4 projection = glm::ortho(-parentX, parentWidth - parentX, parentHeight - parentY, -parentY, -0.1f, 0.1f);
	shader.setUniform("projection", 1, false, glm::value_ptr(projection));
	shader.setUniform("backgroundColor", backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	Rectangle bounds = getBounds();
	//setBounds(bounds.getX() + parentX, bounds.getY() + parentY, bounds.getWidth(), bounds.getHeight());
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	for (std::list<View>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		it->draw(bounds.getX() + parentX, bounds.getY() + parentY, parentWidth, parentHeight);
	}
	//setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void View::addSubview(const View& view) {
	subviews.push_back(view);
}
