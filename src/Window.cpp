#include "Window.h"

const float cgl::Window::SCROLL_SCALE = 5.0f;
bool cgl::Window::firstCall = true;
float cgl::Window::lastX;
float cgl::Window::lastY;

cgl::Window::Window(const std::string& windowName, int w, int h, float r, float g, float b, float a) : backgroundColor(r, g, b, a) {
	window = glfwCreateWindow(w, h, windowName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	view = new View(0, 0, static_cast<float>(width), static_cast<float>(height));
	glfwSetWindowUserPointer(window, view);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, &mouseCallback);
}

cgl::Window::~Window() {
	delete view;
}

cgl::View& cgl::Window::getView() {
	return *view;
}

void cgl::Window::processInput() {
}

void cgl::Window::render() {
	glfwMakeContextCurrent(window);
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	glClear(GL_COLOR_BUFFER_BIT);
	Polygon maxPolygon;
	maxPolygon.addVertex(glm::vec2(-1.0f, 1.0f));
	maxPolygon.addVertex(glm::vec2(1.0f, 1.0f));
	maxPolygon.addVertex(glm::vec2(1.0f, -1.0f));
	maxPolygon.addVertex(glm::vec2(-1.0f, -1.0f));
	view->draw(glm::mat4(), maxPolygon);
	glfwSwapBuffers(window);
}

cgl::Size cgl::Window::getSize() const {
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	return Size(static_cast<float>(width), static_cast<float>(height));
}

cgl::Color cgl::Window::getBackgroundColor() {
	return backgroundColor;
}

void cgl::Window::setBackgroundColor(const Color& bc) {
	backgroundColor = bc;
}

void cgl::Window::close() {
	glfwDestroyWindow(window);
}

bool cgl::Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void cgl::Window::framebufferSizeCallback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
	View* view = static_cast<View*>(glfwGetWindowUserPointer(window));
	Rectangle bounds = view->getBounds();
	view->setBounds(0, 0, static_cast<float>(w), static_cast<float>(h));
}

void cgl::Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	View* view = static_cast<View*>(glfwGetWindowUserPointer(window));
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	int frameWidth = 0, frameHeight = 0;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
	xpos *= frameWidth / width;
	ypos *= frameHeight / height;
	view->scroll(SCROLL_SCALE * xOffset, SCROLL_SCALE * yOffset, xpos, ypos);
}

void cgl::Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	View* view = static_cast<View*>(glfwGetWindowUserPointer(window));
	if (!firstCall) {
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;
		view->onMouseMove(xoffset, yoffset);
	} else {
		firstCall = false;
		lastX = xpos;
		lastY = ypos;
	}
}
