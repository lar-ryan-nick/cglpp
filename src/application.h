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
		virtual ~Application();
	protected:
		virtual void onCreate();
		virtual void onTerminate();
};

void errorCallback(int error, const char* description);

#endif
