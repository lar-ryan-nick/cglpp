#include "View.h"

cgl::Shader* cgl::View::viewShader = NULL;
const unsigned int cgl::View::MAX_VERTICIES = 50;

cgl::View::View(float x, float y, float width, float height) : backgroundColor(0.0f, 0.0f, 0.0f, 0.0f), clipSubviews(false), clipToParent(false), isScrollable(false), isClickable(true), isPressed(false) {
	if (viewShader == NULL) {
		viewShader = new Shader("res/glsl/ViewVertexShader.glsl", "res/glsl/ViewFragmentShader.glsl");
	}
	setBounds(x, y, width, height);
	unsigned int indicies[(MAX_VERTICIES - 2) * 3];
	for (int i = 0; i < (MAX_VERTICIES - 2) * 3; i += 3) {
		indicies[i] = 0;
		indicies[i + 1] = i / 3 + 1;
		indicies[i + 2] = i / 3 + 2;
	}
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_VERTICIES, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
/* TODO: copy buffers on copy and move on move
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
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void cgl::View::setBounds(float x, float y, float width, float height) {
	bounds = Rectangle(x, y, width, height);
	setCenter(glm::vec3(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 2, 0.0f));
}

cgl::Rectangle cgl::View::getBounds() const {
	return bounds;
}

void cgl::View::setBackgroundColor(const Color& bc) {
	backgroundColor = bc;
}

void cgl::View::draw(const glm::mat4& parentModel, const Polygon& poly) {
	Rectangle bounds = getBounds();
	float verticies[8];
	verticies[0] = verticies[6] = bounds.getX();
	verticies[1] = verticies[3] = bounds.getY();
	verticies[2] = verticies[4] = bounds.getX() + bounds.getWidth();
	verticies[5] = verticies[7] = bounds.getY() + bounds.getHeight();
	glm::mat4 model = getTransformationMatrix();
	model = parentModel * model;
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f, -0.1f, 0.1f);
	glm::mat4 mvp = projection * model;
	Polygon p;
	for (int i = 0; i < 8; i += 2) {
		glm::vec4 ver(verticies[i], verticies[i + 1], 0.0f, 1.0f);
		glm::vec4 transformed = mvp * ver;
		p.addVertex(glm::vec2(transformed.x, transformed.y));
	}
	std::list<Polygon> clippedPolygons = p.clipTo(poly);
	for (std::list<Polygon>::iterator it1 = clippedPolygons.begin(); it1 != clippedPolygons.end(); it1++) {
		Polygon clippedPolygon = *it1;
		std::list<Position> vert = clippedPolygon.getVerticies();
		if (vert.size() < 3) {
			continue;
		}
		render(clippedPolygon, model);
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

void cgl::View::render(const cgl::Polygon& bounds, const glm::mat4& model) {
	std::list<Position> vert = bounds.getVerticies();
	std::vector<glm::vec2> v(vert.begin(), vert.end());
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(glm::vec2), &v[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	viewShader->use();
	viewShader->setUniform("backgroundColor", backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	glDrawElements(GL_TRIANGLES, (vert.size() - 2) * 3, GL_UNSIGNED_INT, 0);
	viewShader->finish();
	glBindVertexArray(0);
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

bool cgl::View::scroll(double xOffset, double yOffset, float mouseX, float mouseY) {
	Rectangle bounds = getBounds();
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		glm::mat4 model = view->getInverseTransformationMatrix();
		glm::vec4 pos = model * glm::vec4(mouseX - bounds.getX() + offsetPosition.getX(), mouseY - bounds.getY() + offsetPosition.getY(), 0.0f, 1.0f);
		if (view->getBounds().contains(Position(pos.x, pos.y))) {
			model = glm::mat4(1.0f);
			model = glm::rotate(model, -view->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
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

bool cgl::View::mousePress(int button, int mods, float mouseX, float mouseY) {
	Rectangle bounds = getBounds();
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		glm::mat4 model = view->getInverseTransformationMatrix();
		glm::vec4 pos = model * glm::vec4(mouseX - bounds.getX() + offsetPosition.getX(), mouseY - bounds.getY() + offsetPosition.getY(), 0.0f, 1.0f);
		if (view->getBounds().contains(Position(pos.x, pos.y))) {
			if (view->mousePress(button, mods, pos.x, pos.y)) {
				return true;
			}
		}
	}
	if (isClickable) {
		return onMousePress(button, mods);
	} else {
		return false;
	}
}

bool cgl::View::mouseRelease(int button, int mods, float mouseX, float mouseY) {
	Rectangle bounds = getBounds();
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		glm::mat4 model = view->getInverseTransformationMatrix();
		glm::vec4 pos = model * glm::vec4(mouseX - bounds.getX() + offsetPosition.getX(), mouseY - bounds.getY() + offsetPosition.getY(), 0.0f, 1.0f);
		if (view->getBounds().contains(Position(pos.x, pos.y))) {
			if (view->mouseRelease(button, mods, pos.x, pos.y)) {
				return true;
			}
		}
	}
	if (isClickable) {
		return onMouseRelease(button, mods);
	} else {
		return false;
	}
}

bool cgl::View::onScroll(double xOffset, double yOffset) {
	return false;
}

bool cgl::View::onMousePress(int button, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		isPressed = true;
		return true;
	}
	return false;
}

bool cgl::View::onMouseRelease(int button, int mods) {
	if (isPressed) {
		isPressed = false;
		onClick();
		return true;
	}
	return false;
}

void cgl::View::propogateMouseRelease() {
	isPressed = false;
	for (std::list<View*>::iterator it = subviews.begin(); it != subviews.end(); it++) {
		View* view = *it;
		view->propogateMouseRelease();
	}
}

void cgl::View::onClick() {
	std::cout << "Hi, " << backgroundColor << " has been clicked!" << std::endl;
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
