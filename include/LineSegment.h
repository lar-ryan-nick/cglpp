#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include <cmath>
#include "Position.h"

namespace cgl {
	class LineSegment {
		private:
			Position point1;
			Position point2;
		public:
			LineSegment(const Position& p1, const Position& p2);
			Position getPoint1();
			Position getPoint2();
			float getLength();
			bool intersects(const LineSegment& other, Position& point);
	};
}

#endif