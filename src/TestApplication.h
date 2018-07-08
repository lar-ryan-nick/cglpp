#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include "Application.h"
#include "TestWindow.h"

class TestApplication : public cgl::Application {
	private:
		TestWindow* window;
	public:
		virtual ~TestApplication();
	protected:
		virtual void onCreate();
};

#endif
