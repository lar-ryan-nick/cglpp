#include "ModelViewApplication.h"

ModelViewApplication::ModelViewApplication(const std::string& mp) : modelPath(mp) {}

void ModelViewApplication::onCreate() {
	window = new ModelViewWindow(modelPath);
	addWindow(window);
}

ModelViewApplication::~ModelViewApplication() {
	delete window;
}
