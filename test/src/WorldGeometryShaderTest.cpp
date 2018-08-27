#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../../src/Shader.h"
#include "../../src/Polygon.h"
#include "../../src/Camera.h"
#include "../../src/Actor.h"

unsigned int createShader(const std::string& vertexSourcePath, const std::string& geometrySourcePath);
char* getFileContents(const std::string& filename);

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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	cgl::Shader shader(createShader("res/glsl/WorldVertexShader.glsl", "res/glsl/WorldGeometryShader.glsl"));
	cgl::Polygon container;
	container.addVertex(-0.5, 0.5f);
	container.addVertex(0.5, 0.5f);
	container.addVertex(0.5, -0.5f);
	container.addVertex(-0.5, -0.5f);
	glm::mat4 model;
	cgl::Camera camera(glm::vec3(0.0, 0.0, -3.0));
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 vp = projection * view;
	cgl::Actor actor("res/models/nanosuit/nanosuit.obj");
	/*
	std::vector<glm::vec3> input;
	for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
		for (float y = -1.0f; y <= 1.0f; y += 0.1f) {
			input.push_back(glm::vec3(x, y, 0.0f));
		}
	}
	*/
	std::list<cgl::Mesh*> meshes = actor.getModel().getMeshes();
	std::vector<cgl::Position> positions;
	for (std::list<cgl::Mesh*>::iterator it = meshes.begin(); it != meshes.end() && positions.size() == 0; it++) {
		cgl::Mesh* mesh = *it;
		positions = mesh->getPositions();
	}
	std::vector<glm::vec3> input;
	for (int i = 0; i < positions.size(); i++) {
		input.push_back(static_cast<glm::vec3>(positions[i]));
	}
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * input.size(), &input[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	shader.use();
	unsigned int tbo[2];
	glGenBuffers(2, tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * (input.size()), NULL, GL_STATIC_READ);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, tbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (input.size()), NULL, GL_STATIC_READ);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, tbo[1]);
	while (!glfwWindowShouldClose(window)) {
		glEnable(GL_RASTERIZER_DISCARD);
		glBeginTransformFeedback(GL_TRIANGLES);
		shader.use();
		shader.setUniform("vp", vp);
		shader.setUniform("model", model);
		shader.setUniform("clipRegion[0]", glm::vec2(-.5, .5));
		shader.setUniform("clipRegion[1]", glm::vec2(.5, .5));
		shader.setUniform("clipRegion[2]", glm::vec2(.5, -.5));
		shader.setUniform("clipRegion[3]", glm::vec2(-.5, -.5));
		shader.setUniform("numClipVerticies", 4);
		glDrawArrays(GL_TRIANGLES, 0, input.size());
		shader.finish();
		glEndTransformFeedback();
		glDisable(GL_RASTERIZER_DISCARD);
		glFlush();
		std::vector<glm::vec4> points(input.size(), glm::vec4());
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tbo[0]);
		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(glm::vec4) * points.size(), &points[0]);
		std::vector<int> insides(input.size(), 0);
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tbo[1]);
		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(int) * insides.size(), &insides[0]);
		std::vector<glm::vec4> actuals(input.size(), glm::vec4());
		for (int i = 0; i < actuals.size(); i++) {
			actuals[i] = vp * model * glm::vec4(input[i], 1.0f);
		}
		for (int i = 0; i < actuals.size(); i++) {
			if (points[i] != actuals[i]) {
				std::cout << "Input:  " << cgl::Position(input[i]) << std::endl;
				std::cout << "Actual:  " << cgl::Position(actuals[i]) << std::endl;
				std::cout << "Output: " << cgl::Position(points[i]) << std::endl;
			}
			glm::vec4 coord = actuals[i] / actuals[i].w;
			bool outside = coord.x < -.5 || coord.y < -.5 || coord.x > .5 || coord.y > .5;
			if ((bool)(insides[i]) == outside) {
				std::cout << "GPU FAILED at location " << cgl::Position(coord) << std::endl;
			}
			if (container.contains(cgl::Position(coord)) == outside) {
				std::cout << "CPU FAILED at location " << cgl::Position(coord) << std::endl;
			}
		}
	
		glfwPollEvents();    
	}
	glfwTerminate();
	return 0;
}

unsigned int createShader(const std::string& vertexSourcePath, const std::string& geometrySourcePath) {
	unsigned int id = glCreateProgram();
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char* vertexShaderSource = getFileContents(vertexSourcePath);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	delete[] vertexShaderSource;
	vertexShaderSource = NULL;
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexSourcePath << '\n' << infoLog << std::endl;
	}
	glAttachShader(id, vertexShader);
	unsigned int geometryShader;
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	char* geometryShaderSource = getFileContents(geometrySourcePath);
	glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
	delete[] geometryShaderSource;
	geometryShaderSource = NULL;
	glCompileShader(geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << geometrySourcePath << '\n' << infoLog << std::endl;
	}
	glAttachShader(id, geometryShader);
	const GLchar* feedbackVaryings[] = {"point", "isInside"};
	glTransformFeedbackVaryings(id, 2, feedbackVaryings, GL_SEPARATE_ATTRIBS);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	return id;
}

char* getFileContents(const std::string& filename) {
	std::ifstream fin(filename);
	if (fin.fail()) {
		std::cerr << "Couldn't open file " << filename << '\n';
	}
	fin.seekg(0, std::ios_base::end);
	int characterCount = fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	char* shaderSource = new char[characterCount];
	fin.get(shaderSource, characterCount, '\0');
	fin.close();
	return shaderSource;
}