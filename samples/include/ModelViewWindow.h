#ifndef WORLDTESTWINDOW_H
#define WORLDTESTWINDOW_H

#include "WorldView.h"
#include "Window.h"
#include "Actor.h"
#include <string>

class ModelViewWindow : public cgl::Window {
	private:
		cgl::Actor* actor;
		cgl::WorldView* worldView;
	public:
		ModelViewWindow(const std::string& modelPath);
		virtual ~ModelViewWindow();
		virtual void render();
		virtual void processInput();
};

#endif
