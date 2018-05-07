#include "window.h"

Window::Window(const std::string& windowName, int w, int h, float r, float g, float b, float a) : backgroundColor(r, g, b, a) {
	/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	*/
	window = glfwCreateWindow(w, h, windowName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, &mouseCallback);
	glfwSetFramebufferSizeCallback(window, &framebufferSizeCallback);
	View view(10.0f, 10.0f, 100.0f, 100.0f);
	views.push_back(view);
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
	for (std::list<View>::iterator it = views.begin(); it != views.end(); it++) {
		it->draw(0, 0, width, height);
	}
	glfwSwapBuffers(window);
}

Size Window::getSize() {
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

void framebufferSizeCallback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
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
