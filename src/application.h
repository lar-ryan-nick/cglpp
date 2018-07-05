#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <list>
#include "window.h"

namespace cgl {
	class Application {
		private:
			std::list<Window*> windows;
		public:
			Application();
			void run();
		protected:
			virtual void onCreate() = 0;
			virtual void onTerminate();
			void addWindow(Window* window);
	};

	void errorCallback(int error, const char* description);
}

#endif