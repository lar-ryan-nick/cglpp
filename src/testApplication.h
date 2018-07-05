#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include "application.h"
#include "testWindow.h"

class TestApplication : public cgl::Application {
	protected:
		virtual void onCreate();
		virtual void onTerminate();
};

#endif
