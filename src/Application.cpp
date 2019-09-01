#include "../include/Application.h"

cgl::Application::Application() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw" << std::endl;
		exit(-1);
	}
	glfwSetErrorCallback(&Application::errorCallback);
	glfwSetJoystickCallback(&Application::joystickCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* w = glfwCreateWindow(800, 600, "", NULL, NULL);
	if (w == NULL) {
		std::cerr << "Failed to create initial GLFW window" << std::endl;
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

cgl::Application::~Application() {}

void cgl::Application::run() {
	onCreate();
	// To enter wirefram mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!windows.empty()) {
		for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++) {
			Window* window = *it;
			if (window->shouldClose()) {
				window->close();
				it = windows.erase(it);
				if (windows.empty()) {
					break;
				}
				it--;
			} else {
				window->processInput();
				window->render();
			}
		}
		glfwPollEvents();
		processGamepadInput();
	}
	glfwTerminate();
}

void cgl::Application::addWindow(Window* window) {
	windows.push_back(window);
}

void cgl::Application::processGamepadInput() {
	for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
		if (glfwJoystickIsGamepad(jid)) {
			GLFWgamepadstate state;
			if (glfwGetGamepadState(jid, &state)) {
			}
		} else if (glfwJoystickPresent(jid)) {
			// TODO: add joystick support
		}
	}
}

void cgl::Application::errorCallback(int error, const char* description) {
	std::cerr << "Error Code " << error << ": " << description << std::endl;
}

void cgl::Application::joystickCallback(int joystickID, int event) {
	std::cout << joystickID << std::endl;
	if (event == GLFW_CONNECTED) {
	} else if (event == GLFW_DISCONNECTED) {
	}
}
