#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include "window.h"

class Application {
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

#endif
