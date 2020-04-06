#include "ModelViewWindow.h"

ModelViewWindow::ModelViewWindow(const std::string& modelPath) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	/*
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	*/
	cgl::Rectangle bounds = getView().getBounds();
	worldView = new cgl::WorldView(0, 0, bounds.getWidth(), bounds.getHeight());
	worldView->setBackgroundColor(cgl::Color(0.9f, 0.9f, 0.9f, 1.0f));
	actor = new cgl::Actor(modelPath);
	worldView->addActor(actor);
	getView().addSubview(worldView);
	getView().setBackgroundColor(cgl::Color(0.0f, 0.0f, 0.0f, 1.0f));
}

ModelViewWindow::~ModelViewWindow() {
	delete actor;
	delete worldView;
}

void ModelViewWindow::render() {
	Window::render();
}

void ModelViewWindow::processInput() {
	cgl::Camera camera = worldView->getCamera();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(0.5f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveForward(-0.5f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveRight(-0.5f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(0.5f);
	for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
		if (glfwJoystickIsGamepad(jid)) {
			GLFWgamepadstate state;
			if (glfwGetGamepadState(jid, &state)) {
				float xChange = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
				float yChange = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
				camera.moveRight(xChange * .75f);
				camera.moveForward(yChange * -.75f);
				static float pitch = 0.0f;
				static float yaw = 90.0f;
				xChange = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
				yChange = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
				yaw += xChange;
				if (yaw > 360.0f) {
					yaw -= 360.0f;
				}
				pitch -= yChange;
				if(pitch > 89.0f)
					pitch =  89.0f;
				if(pitch < -89.0f)
					pitch = -89.0f;
				camera.setRotation(pitch, yaw);
			}
		} else if (glfwJoystickPresent(jid)) {
			// TODO: add joystick support
		}
	}
	worldView->setCamera(camera);
}
