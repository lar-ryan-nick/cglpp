#include "application.h"

Application::Application() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw" << std::endl;
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* w = glfwCreateWindow(800, 600, "", NULL, NULL);
	if (w == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(w);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwDestroyWindow(w);
	glfwMakeContextCurrent(NULL);
}

void Application::run() {
	onCreate();
	// To enter wirefram mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/*
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);
	glEnable(GL_CLIP_DISTANCE2);
	glEnable(GL_CLIP_DISTANCE3);
	*/
	while (!windows.empty()) {
		for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++) {
			Window* window = *it;
			if (!window->shouldClose()) {
				window->processInput();
				window->render();
				glfwPollEvents();    
			} else {
				window->close();
				delete window;
				it = windows.erase(it);
				it--;
			}
		}
	}
	onTerminate();
	glfwTerminate();
}

void Application::onTerminate() {
	for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++) {
		delete *it;
	}
}

void Application::addWindow(Window* window) {
	windows.push_back(window);
}

void errorCallback(int error, const char* description) {
	std::cerr << "Error code " << error << ": " << description << std::endl;
}
