#include "WorldTestWindow.h"

WorldTestWindow::WorldTestWindow()  {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cgl::Rectangle bounds = getView().getBounds();
	worldView = new cgl::WorldView(bounds.getWidth() / 4, bounds.getHeight() / 4, bounds.getWidth() / 2, bounds.getHeight() / 2);
	worldView->setBackgroundColor(cgl::Color(0.0f, 0.0f, 0.0f, 1.0f));
	actor = new cgl::Actor("res/models/nanosuit/nanosuit.obj");
	worldView->addActor(actor);
	getView().addSubview(worldView);
	getView().setBackgroundColor(cgl::Color(0.9f, 0.9f, 0.9f, 1.0f));
}

WorldTestWindow::~WorldTestWindow() {
	delete actor;
	delete worldView;
}

void WorldTestWindow::render() {
	//worldView->setRotation(glfwGetTime());
	Window::render();
}

void WorldTestWindow::processInput() {
	cgl::Camera camera = worldView->getCamera();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(0.05f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveForward(-0.05f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveRight(-0.05f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(0.05f);
	worldView->setCamera(camera);
}
