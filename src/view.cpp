#include "view.h"

Shader* View::viewShader = NULL;

View::View(float x, float y, float width, float height) : backgroundColor(0.0f, 0.0f, 0.0f, 0.0f), rotation(0.0f), scalar(1.0f, 1.0f, 1.0f), clipSubviews(false), clipToParent(false), isScrollable(false) {
	if (viewShader == NULL) {
		viewShader = new Shader("res/glsl/viewVertexShader.glsl", "res/glsl/viewFragmentShader.glsl");
	}
	shader = viewShader;
	verticies[0] = verticies[6] = x;
	verticies[1] = verticies[3] = y;
	verticies[2] = verticies[4] = x + width;
	verticies[5] = verticies[7] = y + height;
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};
	unsigned int VBO[2];
	glGenBuffers(2, VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	float textureCoordinates[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
}

View::View(const View& view) : vao(view.vao), backgroundColor(view.backgroundColor), rotation(view.rotation), translation(view.translation), scalar(view.scalar), subviews(view.subviews), clipSubviews(view.clipSubviews), clipToParent(view.clipToParent), isScrollable(view.isScrollable), offsetPosition(view.offsetPosition) {
	viewShader = view.viewShader;
	for (int i = 0; i < 8; i++) {
		verticies[i] = view.verticies[i];
	}
}

View::~View() {
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		delete *it;
	}
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
	shader->use();
	Rectangle bounds = getBounds();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(bounds.getX(), bounds.getY(), 0.0f));
	model = glm::scale(model, scalar);
	model = glm::translate(model, glm::vec3(bounds.getWidth() / 2, bounds.getHeight() / 2, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-bounds.getX() - bounds.getWidth() / 2, -bounds.getY() - bounds.getHeight() / 2, 0.0f));
	model = glm::translate(model, translation);
	shader->setUniform("model", 1, false, glm::value_ptr(model));
	glm::mat4 projection = glm::ortho(-parentX, parentWidth - parentX, parentHeight - parentY, -parentY, -0.1f, 0.1f);
	shader->setUniform("projection", 1, false, glm::value_ptr(projection));
	shader->setUniform("backgroundColor", backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		if (view->getClipToParent() || clipSubviews) {
			float viewport[4];
			glGetFloatv(GL_VIEWPORT, viewport);
			float viewWidth = bounds.getWidth();
			if (parentX + bounds.getX() + viewWidth > parentWidth) {
				viewWidth -= parentWidth;
				if (viewWidth < 0) {
					viewWidth = 0;
				}
			}
			float viewHeight = bounds.getHeight();
			if (parentY + bounds.getY() + viewHeight > parentHeight) {
				viewHeight -= parentHeight;
				if (viewHeight < 0) {
					viewHeight = 0;
				}
			}
			glViewport(parentX + bounds.getX(), parentHeight - bounds.getY() - bounds.getHeight(), viewWidth, viewHeight);
			view->draw(-offsetPosition.getX(), -offsetPosition.getY(), viewWidth, viewHeight);
			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		} else {
			view->draw(bounds.getX() + parentX, bounds.getY() + parentY, parentWidth, parentHeight);
		}
	}
	shader->finish();
}

void View::addSubview(View* view) {
	subviews.push_back(view);
}

void View::setClipSubviews(bool clipSub) {
	clipSubviews = clipSub;
}

void View::setClipToParent(bool clipToP) {
	clipToParent = clipToP;
}

bool View::getClipToParent() const {
	return clipToParent;
}

void View::setIsScrollable(bool s) {
	isScrollable = s;
}

bool View::getIsScrollable() const {
	return isScrollable;
}

void View::scroll(double xOffset, double yOffset) {
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		view->scroll(xOffset, yOffset);
	}
	if (isScrollable) {
		onScroll(xOffset, yOffset);
	}
}

void View::onScroll(double xOffset, double yOffset) {
}

void View::translate(float x, float y) {
	translation += glm::vec3(x, y, 0.0f);
}

void View::setTranslation(float x, float y) {
	translation = glm::vec3(x, y, 0.0f);
}

void View::rotate(float radians) {
	rotation += radians;
}

void View::setRotation(float radians) {
	rotation = radians;
}

void View::scale(float x, float y) {
	scalar += glm::vec3(x, y, 0.0f);
}

void View::setScalar(float x, float y) {
	scalar = glm::vec3(x, y, 0.0f);
}

Position View::getOffsetPosition() const {
	return offsetPosition;
}

void View::setOffsetPosition(const Position& offset) {
	offsetPosition = offset;
}
