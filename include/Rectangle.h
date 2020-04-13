#ifndef CGL_RECTANGLE_H
#define CGL_RECTANGLE_H

#include "Position.h"
#include "Size.h"

namespace cgl {
	class Rectangle {
		public:
			Rectangle(float xpos = 0.0f, float ypos = 0.0f, float w = 0.0f, float h = 0.0f);
			Rectangle(const Position& p, const Size& s);
			Position getOrigin() const;
			Size getSize() const;
			float getX() const;
			float getY() const;
			float getWidth() const;
			float getHeight() const;
			bool contains(const Position& p) const;
		private:
			Position origin;
			Size size;
			bool isInbetween(float min, float max, float val) const;
	};
}

#endif
