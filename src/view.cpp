#include "view.h"

Shader* View::viewShader = NULL;

View::View(float x, float y, float width, float height) : backgroundColor(0.0f, 0.0f, 0.0f, 0.0f), rotation(0.0f), scalar(1.0f, 1.0f, 1.0f), clipSubviews(false), clipToParent(false), isScrollable(false) {
	if (viewShader == NULL) {
		viewShader = new Shader("res/glsl/viewVertexShader.glsl", "res/glsl/viewFragmentShader.glsl");
	}
	shader = viewShader;
	bounds = Rectangle(x, y, width, height);
	float verticies[8];
	verticies[0] = verticies[6] = 0.0f;
	verticies[1] = verticies[3] = 0.0f;
	verticies[2] = verticies[4] = 0.5f;
	verticies[5] = verticies[7] = 0.5f;
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};
	float textureCoordinates[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	glGenBuffers(2, vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	/*
	verticies[0] = verticies[6] = x;
	verticies[1] = verticies[3] = y;
	verticies[2] = verticies[4] = x + width;
	verticies[5] = verticies[7] = y + height;
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};
	glGenBuffers(2, vbo);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	float textureCoordinates[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	*/
}
/* fix later
View::View(const View& view) : vao(view.vao), backgroundColor(view.backgroundColor), rotation(view.rotation), translation(view.translation), scalar(view.scalar), subviews(view.subviews), clipSubviews(view.clipSubviews), clipToParent(view.clipToParent), isScrollable(view.isScrollable), offsetPosition(view.offsetPosition) {
	viewShader = view.viewShader;
	for (int i = 0; i < 8; i++) {
		verticies[i] = view.verticies[i];
	}
}
*/
View::~View() {
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		delete *it;
	}
	// add buffer data deletion
}

void View::setBounds(float x, float y, float width, float height) {
	bounds = Rectangle(x, y, width, height);
	/*
	verticies[0] = verticies[6] = x;
	verticies[1] = verticies[3] = y;
	verticies[2] = verticies[4] = x + width;
	verticies[5] = verticies[7] = y + height;
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies), verticies);
	*/
}

Rectangle View::getBounds() const {
	return bounds;//return Rectangle(verticies[0], verticies[1], verticies[2] - verticies[0], verticies[5] - verticies[1]);
}

void View::setBackgroundColor(const Color& bc) {
	backgroundColor = bc;
}

void View::draw(float parentX, float parentY, float parentWidth, float parentHeight, const glm::mat4& parentModel, const Polygon& poly) {
	Rectangle bounds = getBounds();
	float verticies[8];
	verticies[0] = verticies[6] = bounds.getX();
	verticies[1] = verticies[3] = bounds.getY();
	verticies[2] = verticies[4] = bounds.getX() + bounds.getWidth();
	verticies[5] = verticies[7] = bounds.getY() + bounds.getHeight();
	glm::mat4 model;
	model = glm::translate(model, translation);
	model = glm::translate(model, glm::vec3(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scalar);
	model = glm::translate(model, glm::vec3(-bounds.getX() - bounds.getWidth() / 2, -bounds.getY() - bounds.getHeight() / 2, 0.0f));
	/*
	float minX = parentX;
	float maxX = parentX + parentWidth;
	float minY = parentY;
	float maxY = parentY + parentHeight;
	*/
	model = parentModel * model;
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f, -0.1f, 0.1f);
	Polygon p;
	for (int i = 0; i < 8; i += 2) {
		glm::vec4 ver(verticies[i], verticies[i + 1], 0.0f, 1.0f);
		glm::vec4 transformed = projection * model * ver;
		verticies[i] = transformed.x;
		verticies[i + 1] = transformed.y;
		p.addVertex(glm::vec2(verticies[i], verticies[i + 1]));
	}
	float indicies[6];
	for (int i = 0; i < (8 / 2 - 2) * 3; i += 3) {
		indicies[i] = 0;
		indicies[i + 1] = i / 3 + 1;
		indicies[i + 2] = i / 3 + 2;
	}
	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies), verticies);
	shader->use();
	/*
	shader->setUniform("clipRect", minX, minY, maxX, maxY);
	shader->setUniform("model", 1, false, glm::value_ptr(model));
	shader->setUniform("parentModel", 1, false, glm::value_ptr(parentModel));
	shader->setUniform("projection", 1, false, glm::value_ptr(projection));
	*/
	shader->setUniform("backgroundColor", backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	shader->finish();
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		view->translate(bounds.getX() - offsetPosition.getX(), bounds.getY() - offsetPosition.getY());
		if (view->getClipToParent() || clipSubviews) {
			/*
			if (parWidth > 0 && parHeight > 0) {
				view->draw(parX, parY, parWidth, parHeight, parentModel * model);
			}
			*/
			view->draw(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), model, p);
		} else {
			view->draw(parentX, parentY, parentWidth, parentHeight, model, p);
		}
		view->translate(offsetPosition.getX() - bounds.getX(), offsetPosition.getY() - bounds.getY());
	}
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

bool View::scroll(double xOffset, double yOffset, float mouseX, float mouseY) {
	Rectangle bounds = getBounds();
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		Rectangle subBounds = view->getBounds();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(subBounds.getX() + subBounds.getWidth() / 2, subBounds.getY() + subBounds.getHeight() / 2, 0.0f));
		model = glm::scale(model, glm::vec3(1 / view->scalar.x, 1 / view->scalar.y, 1.0f));
		model = glm::rotate(model, -view->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-subBounds.getX() - subBounds.getWidth() / 2, -subBounds.getY() - subBounds.getHeight() / 2, 0.0f));
		model = glm::translate(model, -view->translation);
		glm::vec4 pos = model * glm::vec4(mouseX - bounds.getX() + offsetPosition.getX(), mouseY - bounds.getY() + offsetPosition.getY(), 0.0f, 1.0f);
		if (subBounds.contains(Position(pos.x, pos.y))) {
			model = glm::mat4();
			model = glm::rotate(model, -view->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::vec4 rot = model * glm::vec4(xOffset, yOffset, 0.0f, 1.0f);
			if (view->scroll(rot.x, rot.y, pos.x, pos.y)) {
				return true;
			}
		}
	}
	if (isScrollable) {
		return onScroll(xOffset, yOffset);
	} else {
		return false;
	}
}

bool View::onScroll(double xOffset, double yOffset) {
	return false;
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
	scalar *= glm::vec3(x, y, 0.0f);
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
