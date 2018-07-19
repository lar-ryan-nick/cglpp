#include "View.h"

cgl::Shader* cgl::View::viewShader = NULL;
const unsigned int cgl::View::MAX_VERTICIES = 50;

cgl::View::View(float x, float y, float width, float height) : bounds(x, y, width, height), backgroundColor(0.0f, 0.0f, 0.0f, 0.0f), rotation(0.0f), clipSubviews(false), clipToParent(false), isScrollable(false) {
	if (viewShader == NULL) {
		viewShader = new Shader("res/glsl/ViewVertexShader.glsl", "res/glsl/ViewFragmentShader.glsl");
	}
	shader = viewShader;
	unsigned int indicies[(MAX_VERTICIES - 2) * 3];
	for (int i = 0; i < (MAX_VERTICIES - 2) * 3; i += 3) {
		indicies[i] = 0;
		indicies[i + 1] = i / 3 + 1;
		indicies[i + 2] = i / 3 + 2;
	}
	glGenBuffers(2, vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * MAX_VERTICIES, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * MAX_VERTICIES, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
/* fix later
View::View(const View& view) : vao(view.vao), backgroundColor(view.backgroundColor), rotation(view.rotation), translation(view.translation), scalar(view.scalar), subviews(view.subviews), clipSubviews(view.clipSubviews), clipToParent(view.clipToParent), isScrollable(view.isScrollable), offsetPosition(view.offsetPosition) {
	viewShader = view.viewShader;
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	for (int i = 0; i < 8; i++) {
		verticies[i] = view.verticies[i];
	}
}
*/
cgl::View::~View() {
	/* want users to delete their own subviews now
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		delete *it;
	}
	*/
	// buffer data deletion
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void cgl::View::setBounds(float x, float y, float width, float height) {
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

cgl::Rectangle cgl::View::getBounds() const {
	return bounds;//return Rectangle(verticies[0], verticies[1], verticies[2] - verticies[0], verticies[5] - verticies[1]);
}

void cgl::View::setBackgroundColor(const Color& bc) {
	backgroundColor = bc;
}

glm::mat4 cgl::View::getTransformationMatrix() const {
	glm::mat4 model;
	model = glm::translate(model, static_cast<glm::vec3>(translation));
	model = glm::translate(model, glm::vec3(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, static_cast<glm::vec3>(scalar));
	model = glm::translate(model, glm::vec3(-bounds.getX() - bounds.getWidth() / 2, -bounds.getY() - bounds.getHeight() / 2, 0.0f));
	return model;
}

void cgl::View::draw(const glm::mat4& parentModel, const Polygon& poly) {
	Rectangle bounds = getBounds();
	float verticies[8];
	verticies[0] = verticies[6] = bounds.getX();
	verticies[1] = verticies[3] = bounds.getY();
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	verticies[2] = verticies[4] = bounds.getX() + bounds.getWidth();
	verticies[5] = verticies[7] = bounds.getY() + bounds.getHeight();
	glm::mat4 model = getTransformationMatrix();
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
		p.addVertex(glm::vec2(transformed.x, transformed.y));
	}
	float a = (1 + (verticies[1] - verticies[5]) / (verticies[3] - verticies[1])) / (verticies[4] - verticies[0] + (verticies[5] - verticies[1]) * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]));
	float b = (1 + a * (verticies[0] - verticies[2])) / (verticies[3] - verticies[1]);
	float d = -a * verticies[0] - b * verticies[1];
	float e = -1 / (verticies[4] - verticies[0] + (verticies[5] - verticies[1]) * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]));
	float f = e * (verticies[0] - verticies[2]) / (verticies[3] - verticies[1]);
	float h = 1 - e * verticies[0] - f * verticies[1];
	glm::mat4 textureMapper(a, e, 0.0f, 0.0f, b, f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, d, h, 0.0f, 0.0f);
	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		Polygon clippedPolygon = *it1;
		std::list<Position> vert = clippedPolygon.getVerticies();
		if (vert.size() < 3) {
			continue;
		}
		float* v = new float[vert.size() * 2];
		float* t = new float[vert.size() * 2];
		int index = 0;
		for (std::list<Position>::iterator it2 = vert.begin(); it2 != vert.end(); it2++) {
			v[index] = it2->getX();
			v[index + 1] = it2->getY();
			glm::vec4 textureTransform = textureMapper * *it2;
			t[index] = textureTransform.x;
			t[index + 1] = textureTransform.y;
			index += 2;
		}
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * vert.size(), v);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * vert.size(), t);
		delete[] v;
		delete[] t;
		shader->use();
		shader->setUniform("backgroundColor", backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
		glDrawElements(GL_TRIANGLES, (vert.size() - 2) * 3, GL_UNSIGNED_INT, 0);
		shader->finish();
		glBindVertexArray(0);
		for (std::list<View*>::iterator it2 = subviews.begin(); it2 != subviews.end(); it2++) {
			View* view = *it2;
			view->translate(bounds.getX() - offsetPosition.getX(), bounds.getY() - offsetPosition.getY());
			if (view->getClipToParent() || clipSubviews) {
				view->draw(model, clippedPolygon);
			} else {
				view->draw(model, p);
			}
			view->translate(offsetPosition.getX() - bounds.getX(), offsetPosition.getY() - bounds.getY());
		}
	}
}

void cgl::View::addSubview(View* view) {
	subviews.push_back(view);
}

void cgl::View::setClipSubviews(bool clipSub) {
	clipSubviews = clipSub;
}

void cgl::View::setClipToParent(bool clipToP) {
	clipToParent = clipToP;
}

bool cgl::View::getClipToParent() const {
	return clipToParent;
}

void cgl::View::setIsScrollable(bool s) {
	isScrollable = s;
}

bool cgl::View::getIsScrollable() const {
	return isScrollable;
}

glm::mat4 cgl::View::getInverseTransformationMatrix() const {
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2, 0.0f));
	model = glm::scale(model, glm::vec3(1 / scalar.getX(), 1 / scalar.getY(), 1.0f));
	model = glm::rotate(model, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-bounds.getX() - bounds.getWidth() / 2, -bounds.getY() - bounds.getHeight() / 2, 0.0f));
	model = glm::translate(model, static_cast<glm::vec3>(-translation));
	return model;
}

bool cgl::View::scroll(double xOffset, double yOffset, float mouseX, float mouseY) {
	Rectangle bounds = getBounds();
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		glm::mat4 model = view->getInverseTransformationMatrix();
		glm::vec4 pos = model * glm::vec4(mouseX - bounds.getX() + offsetPosition.getX(), mouseY - bounds.getY() + offsetPosition.getY(), 0.0f, 1.0f);
		if (view->getBounds().contains(Position(pos.x, pos.y))) {
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

bool cgl::View::onScroll(double xOffset, double yOffset) {
	return false;
}

void cgl::View::translate(float x, float y) {
	translation += glm::vec3(x, y, 0.0f);
}

void cgl::View::setTranslation(float x, float y) {
	translation = glm::vec3(x, y, 0.0f);
}

void cgl::View::rotate(float radians) {
	rotation += radians;
}

void cgl::View::setRotation(float radians) {
	rotation = radians;
}

void cgl::View::scale(float x, float y) {
	scalar *= glm::vec3(x, y, 0.0f);
}

void cgl::View::setScalar(float x, float y) {
	scalar = glm::vec3(x, y, 0.0f);
}

cgl::Position cgl::View::getOffsetPosition() const {
	return offsetPosition;
}

void cgl::View::setOffsetPosition(const Position& offset) {
	offsetPosition = offset;
}

void cgl::View::onMouseMove(float xOffset, float yOffset) {
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* subview = *it;
		subview->onMouseMove(xOffset, yOffset);
	}
}
