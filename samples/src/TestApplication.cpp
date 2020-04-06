#include "../include/TestApplication.h"

void TestApplication::onCreate() {
	//window = new TestWindow(); // 2D
	window = new WorldTestWindow(); // 3D
	addWindow(window);
}

TestApplication::~TestApplication() {
	delete window;
}
