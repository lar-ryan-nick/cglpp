#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glad/glad.h>
#include "Position.h"
#include "Size.h"

namespace cgl {
	class Rectangle {
		Position origin;
		Size size;
		bool isInbetween(float min, float max, float val) const;
		public:
			Rectangle(float xpos, float ypos, float w, float h);
			Rectangle(const Position& p, const Size& s);
			Position getOrigin() const;
			Size getSize() const;
			float getX() const;
			float getY() const;
			float getWidth() const;
			float getHeight() const;
			bool contains(const Position& p) const;
	};
}

#endif
