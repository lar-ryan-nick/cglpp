#include "TestApplication.h"

void TestApplication::onCreate() {
	window = new TestWindow();
	addWindow(window);
}

TestApplication::~TestApplication() {
	delete window;
}
