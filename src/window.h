#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <string>
#include "size.h"
#include "color.h"
#include "view.h"
#include "scrollView.h"
#include "imageView.h"

namespace cgl {
	class Window {
		private:
			GLFWwindow* window;
			Color backgroundColor;
			View* view;
			const static float SCROLL_SCALE;
			static void framebufferSizeCallback(GLFWwindow* window, int w, int h);
			static void scrollCallback(GLFWwindow* w, double xOffset, double yOffset);
			static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
		public:
			Window(const std::string& windowName = "Application", int w = 1280, int h = 720, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
			virtual ~Window();
			void processInput();
			virtual void render();
			Size getSize() const;
			View& getView();
			void setBackgroundColor(const Color& color);
			Color getBackgroundColor();
			//void scroll(double xOffset, double yOffset);
			bool shouldClose();
			void close();
		protected:
			//virtual b onScroll(double xOffset, double yOffset);
	};
}

#endif
