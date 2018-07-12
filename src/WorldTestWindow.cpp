#include "WorldTestWindow.h"

WorldTestWindow::WorldTestWindow()  {
	cgl::Rectangle bounds = getView().getBounds();
	worldView = new cgl::WorldView(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	actor = new cgl::Actor("res/models/crysis-nano-suit-2/source/fbx/scene.fbx");
	worldView->addActor(actor);
	getView().addSubview(worldView);
}

WorldTestWindow::~WorldTestWindow() {
	delete actor;
	delete worldView;
}
