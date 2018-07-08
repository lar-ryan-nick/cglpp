#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <list>
#include "Window.h"

namespace cgl {
	class Application {
		private:
			std::list<Window*> windows;
			//static void errorCallback(int error, const char* description);
		public:
			Application();
			virtual ~Application();
			void run();
		protected:
			virtual void onCreate() = 0;
			void addWindow(Window* window);
	};
}

#endif