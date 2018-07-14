#include "WorldTestWindow.h"

WorldTestWindow::WorldTestWindow()  {
	cgl::Rectangle bounds = getView().getBounds();
	worldView = new cgl::WorldView(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	actor = new cgl::Actor("res/models/nanosuit/nanosuit.obj");
	worldView->addActor(actor);
	worldView->setBackgroundColor(cgl::Color(0.0f, 1.0f, 0.0f));
	getView().addSubview(worldView);
	getView().setBackgroundColor(cgl::Color(1.0f, 0.0f, 0.0f));
	setBackgroundColor(cgl::Color(0.0f, 0.0f, 1.0f));
}

WorldTestWindow::~WorldTestWindow() {
	delete actor;
	delete worldView;
}

void WorldTestWindow::processInput() {
	cgl::Camera camera = worldView->getCamera();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveForward(-1);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveRight(-1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(1);
	worldView->setCamera(camera);
}
