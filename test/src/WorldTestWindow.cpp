#include "../include/WorldTestWindow.h"

WorldTestWindow::WorldTestWindow()  {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cgl::Rectangle bounds = getView().getBounds();
	worldView = new cgl::WorldView(0, 0, bounds.getWidth(), bounds.getHeight());
	worldView->setBackgroundColor(cgl::Color(0.9f, 0.9f, 0.9f, 1.0f));
	//actor = new cgl::Actor("res/models/Lamborginhi Aventador FBX/Lamborghini_Aventador.fbx");
	//actor = new cgl::Actor("res/models/nanosuit/nanosuit.obj");
	//actor = new cgl::Actor("res/models/maw_j_laygo.fbx");
	//actor = new cgl::Actor("res/models/Looking Around.fbx");
	actor = new cgl::Actor("res/models/Samba Dancing.fbx");
	worldView->addActor(actor);
	getView().addSubview(worldView);
	getView().setBackgroundColor(cgl::Color(0.0f, 0.0f, 0.0f, 1.0f));
}

WorldTestWindow::~WorldTestWindow() {
	delete actor;
	delete worldView;
}

void WorldTestWindow::render() {
	Window::render();
}

void WorldTestWindow::processInput() {
	cgl::Camera camera = worldView->getCamera();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(0.5f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveForward(-0.5f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveRight(-0.5f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(0.5f);
	worldView->setCamera(camera);
}
