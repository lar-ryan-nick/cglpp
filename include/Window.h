#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <string>
#include "Size.h"
#include "Color.h"
#include "View.h"
#include "ScrollView.h"
#include "ImageView.h"

namespace cgl {
	class Window {
		private:
			Color backgroundColor;
			View* view;
			bool firstCall;
			float lastX;
			float lastY;
			const static float SCROLL_SCALE;
			static void framebufferSizeCallback(GLFWwindow* window, int w, int h);
			static void scrollCallback(GLFWwindow* w, double xOffset, double yOffset);
			static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
			static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		public:
			Window(const std::string& windowName = "Application", int w = 1280, int h = 720, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
			virtual ~Window();
			void linkCallbacks();
			// remove virtual on input later
			virtual void processInput();
			virtual void render();
			Size getSize() const;
			View& getView();
			void setBackgroundColor(const Color& color);
			Color getBackgroundColor();
			bool shouldClose();
			void close();
		protected:
			// make private later
			GLFWwindow* window;
	};
}

#endif
