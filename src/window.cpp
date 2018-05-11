#include "window.h"

Window::Window(const std::string& windowName, int w, int h, float r, float g, float b, float a) : backgroundColor(r, g, b, a) {
	window = glfwCreateWindow(w, h, windowName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	view = new View(0, 0, width, height);
	view->setBackgroundColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	ScrollView* scrollView = new ScrollView(100, 100, 650, 450);
	scrollView->setBackgroundColor(Color(0.0f, 0.0f, 1.0f, 1.0f));
	scrollView->setContentSize(1000, 1000);
	View* subview = new View(250, 250, 500, 500);
	subview->setBackgroundColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
	scrollView->addSubview(subview);
	view->addSubview(scrollView);

	glfwSetWindowUserPointer(window, view);
	glfwSetScrollCallback(window, scrollCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, &mouseCallback);
}

Window::~Window() {
	delete view;
}

void Window::processInput() {
}

void Window::render() {
	glfwMakeContextCurrent(window);
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
	glClear(GL_COLOR_BUFFER_BIT);
	view->draw(0, 0, width, height);
	glfwSwapBuffers(window);
}

Size Window::getSize() const {
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	return Size(width, height);
}

Color Window::getBackgroundColor() {
	return backgroundColor;
}

void Window::setBackgroundColor(const Color& bc) {
	backgroundColor = bc;
}

void Window::close() {
	glfwDestroyWindow(window);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void Window::scrollCallback(GLFWwindow* w, double xOffset, double yOffset) {
	View* view = static_cast<View*>(glfwGetWindowUserPointer(w));
	view->scroll(xOffset, yOffset);
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstCall = true;
	static float lastX = xpos;
	static float lastY = ypos;
	if (!firstCall) {
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;
	} else {
		firstCall = false;
	}
}
