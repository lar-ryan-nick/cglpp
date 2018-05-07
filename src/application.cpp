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
	GLFWwindow* window = glfwCreateWindow(800, 600, "", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwDestroyWindow(window);
	glfwMakeContextCurrent(NULL);
	onCreate();
	while (!windows.empty()) {
		for (std::list<Window>::iterator it = windows.begin(); it != windows.end(); it++) {
			if (!it->shouldClose()) {
				it->processInput();
				it->render();
				glfwPollEvents();    
			} else {
				it->close();
				windows.erase(it);
			}
		}
	}
	onTerminate();
	glfwTerminate();
}

void Application::onCreate() {
	Window window1;
	Color color1(0.0f, 0.0f, 1.0f, 1.0f);
	window1.setBackgroundColor(color1);
	windows.push_back(window1);
	Window window2;
	Color color2(0.0f, 1.0f, 0.0f, 1.0f);
	window2.setBackgroundColor(color2);
	windows.push_back(window2);
	Window window3;
	Color color3(1.0f, 1.0f, 1.0f, 1.0f);
	window3.setBackgroundColor(color3);
	windows.push_back(window3);
}

void Application::onTerminate() {
}

void errorCallback(int error, const char* description) {
	std::cerr << "Error code " << error << ": " << description << std::endl;
}
