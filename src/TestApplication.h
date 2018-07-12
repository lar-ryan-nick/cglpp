#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include "Application.h"
#include "TestWindow.h"
#include "WorldTestWindow.h"

class TestApplication : public cgl::Application {
	private:
		cgl::Window* window;
	public:
		virtual ~TestApplication();
	protected:
		virtual void onCreate();
};

#endif
