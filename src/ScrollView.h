#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "View.h"
#include "Position.h"
#include "Size.h"

namespace cgl {
	class ScrollView : public View {
		private:
			Size contentSize;
			void fixContentSize();
		public:
			ScrollView(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
			void setContentSize(float width, float height);
			void setContentSize(const Size& s);
		protected:
			virtual bool onScroll(double xOffset, double yOffset);
	};
}

#endif
