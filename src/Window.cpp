#include "../include/Window.h"
#include "Font.h"

const float cgl::Window::SCROLL_SCALE = 5.0f;

cgl::Window::Window(const std::string& windowName, int w, int h, float r, float g, float b, float a) : backgroundColor(r, g, b, a), firstCall(true) {
	window = glfwCreateWindow(w, h, windowName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
<<<<<<< HEAD
	int wid = 0, hei, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/img/logo.png", &wid, &hei, &nrChannels, 0);
	GLFWimage logo;
	logo.width = wid;
	logo.height = hei;
	logo.pixels = data;
	//std::cout << wid << std::endl;
	glfwSetWindowIcon(window, 1, &logo);
=======
>>>>>>> parent of 876826a... Changed default window icon?
	glfwMakeContextCurrent(window);
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	view = new View(0, 0, static_cast<float>(width), static_cast<float>(height));
}

void cgl::Window::linkCallbacks() {
	glfwSetWindowUserPointer(window, this);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void cgl::Window::framebufferSizeCallback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
	window->view->setBounds(0, 0, static_cast<float>(width), static_cast<float>(height));
}

void cgl::Window::scrollCallback(GLFWwindow* w, double xOffset, double yOffset) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
	double xpos, ypos;
	glfwGetCursorPos(w, &xpos, &ypos);
	int width = 0, height = 0;
	glfwGetWindowSize(w, &width, &height);
	int frameWidth = 0, frameHeight = 0;
	glfwGetFramebufferSize(w, &frameWidth, &frameHeight);
	xpos *= frameWidth / width;
	ypos *= frameHeight / height;
	window->view->scroll(SCROLL_SCALE * xOffset, SCROLL_SCALE * yOffset, xpos, ypos);
}

void cgl::Window::mouseMoveCallback(GLFWwindow* w, double xpos, double ypos) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
	if (!window->firstCall) {
		float xoffset = xpos - window->lastX;
		float yoffset = window->lastY - ypos; // reversed since y-coordinates range from bottom to top
		window->lastX = xpos;
		window->lastY = ypos;
		window->view->onMouseMove(xoffset, yoffset);
	} else {
		window->firstCall = false;
		window->lastX = xpos;
		window->lastY = ypos;
	}
}

void cgl::Window::mouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
	double xpos, ypos;
	glfwGetCursorPos(w, &xpos, &ypos);
	switch (action) {
		case GLFW_PRESS:
			window->view->mousePress(button, mods, xpos, ypos);
			break;
		case GLFW_RELEASE:
			window->view->mouseRelease(button, mods, xpos, ypos);
			window->view->propogateMouseRelease();
			break;
		default:
			break;
	}
}
