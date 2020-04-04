#ifndef MODELVIEWAPPLICATION_H
#define MODELVIEWAPPLICATION_H

#include <string>
#include <Application.h>
#include "ModelViewWindow.h"

class ModelViewApplication : public cgl::Application {
	private:
		std::string modelPath;
		cgl::Window* window;
	public:
		ModelViewApplication(const std::string& mp);
		virtual ~ModelViewApplication();
	protected:
		virtual void onCreate();
};

#endif
