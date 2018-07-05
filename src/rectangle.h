#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glad/glad.h>
#include "position.h"
#include "size.h"

namespace cgl {
	class Rectangle : public Position, public Size {
		bool isInbetween(float min, float max, float val) const;
		public:
			Rectangle(float xpos = 0.0f, float ypos = 0.0f, float w = 1.0f, float h = 1.0f);
			bool contains(Position p) const;
	};
}

#endif
