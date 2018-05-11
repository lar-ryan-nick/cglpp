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
	onCreate();
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

Application::~Application() {
	for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++) {
		delete *it;
	}
}

void Application::onCreate() {
	Window* window = new Window();
	Color color(1.0f, 1.0f, 1.0f, 1.0f);
	window->setBackgroundColor(color);
	windows.push_back(window);
}

void Application::onTerminate() {
}

void errorCallback(int error, const char* description) {
	std::cerr << "Error code " << error << ": " << description << std::endl;
}
