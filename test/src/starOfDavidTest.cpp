#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "../src/Shader.h"
#include "../src/Texture.h"
#include "../src/Camera.h"
#include "../src/StarOfDavid.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
int width = 600, height = 600;
cgl::Camera camera(glm::vec3(0.0f, 0.0f, -3.0f));

int main() {
	if (!glfwInit()) {
		// Initialization failed
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	cgl::Shader shader("res/glsl/starOfDavidTestVertexShader.glsl", "res/glsl/starOfDavidTestFragmentShader.glsl");
	cgl::StarOfDavid sod;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 model;
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setUniform("model", 1, false, glm::value_ptr(model));
		shader.setUniform("view", 1, false, glm::value_ptr(camera.getViewMatrix()));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		shader.setUniform("projection", 1, false, glm::value_ptr(projection));
		sod.bindVAO();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();    
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveForward(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveRight(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(cameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstCall = true;
	static float lastX = xpos;
	static float lastY = ypos;
	static float pitch = 0.0f;
	static float yaw = 90.0f;
	if (!firstCall) {
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.15f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		if (yaw > 360.0f) {
			yaw -= 360.0f;
		}
		pitch += yoffset;
		if(pitch > 89.0f)
			pitch =  89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;
		camera.setRotation(pitch, yaw);
	} else {
		firstCall = false;
	}
}
