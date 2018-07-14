#ifndef WORLDTESTWINDOW_H
#define WORLDTESTWINDOW_H

#include "WorldView.h"
#include "Window.h"
#include "Actor.h"

class WorldTestWindow : public cgl::Window {
	private:
		cgl::WorldView* worldView;
		cgl::Actor* actor;
	public:
		WorldTestWindow();
		virtual ~WorldTestWindow();
		void processInput();
};

#endif
