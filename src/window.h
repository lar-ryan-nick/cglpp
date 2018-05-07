#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <string>
#include "size.h"
#include "color.h"
#include "view.h"

class Window {
	GLFWwindow* window;
	Color backgroundColor;
	std::list<View> views;
	public:
		Window(const std::string& windowName = "Application", int w = 1280, int h = 720, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
		void processInput();
		virtual void render();
		Size getSize();
		void setBackgroundColor(const Color& color);
		Color getBackgroundColor();
		bool shouldClose();
		void close();
};

void framebufferSizeCallback(GLFWwindow* window, int w, int h);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

#endif
