#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "view.h"
#include "position.h"
#include "size.h"

class ScrollView : public View {
	Size contentSize;
	public:
		ScrollView(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
		void setContentSize(float width, float height);
	//protected:
		virtual void onScroll(double xOffset, double yOffset);
};

#endif
