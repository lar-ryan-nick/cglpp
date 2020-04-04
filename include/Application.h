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
			static void errorCallback(int error, const char* description);
			static void joystickCallback(int joystickID, int event);
			void processGamepadInput();
		public:
			Application();
			virtual ~Application();
			void run();
			void addWindow(Window* window);
		protected:
			virtual void onCreate() = 0;
	};
}

#endif
