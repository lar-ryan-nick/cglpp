#ifndef WORLDTESTWINDOW_H
#define WORLDTESTWINDOW_H

#include <WorldView.h>
#include <Window.h>
#include <Actor.h>
#include <Animation.h>
#include <PlanarMesh.h>

class WorldTestWindow : public cgl::Window {
	private:
		cgl::Actor* actor1;
		cgl::Actor* actor2;
		cgl::Animation* animation;
		cgl::WorldView* worldView;
		float animationStart;
	public:
		WorldTestWindow();
		virtual ~WorldTestWindow();
		virtual void render();
		virtual void processInput();
};

#endif
