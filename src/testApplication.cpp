#include "testApplication.h"

void TestApplication::onCreate() {
	TestWindow* window = new TestWindow();
	addWindow(window);
}

void TestApplication::onTerminate() {
	cgl::Application::onTerminate();
}
