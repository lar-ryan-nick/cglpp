#ifndef POSITION_H
#define POSITION_H

#include <functional>

namespace cgl {
	class Position {
		private:
			float x;
			float y;
		public:
			Position(float xPos = 0.0f, float yPos = 0.0f);
			float getX() const;
			float getY() const;
			void setX(float xPos);
			void setY(float yPos);
			void move(float xOffset, float yOffset);
			bool operator==(const Position& p) const;
	};
}

namespace std {
	template<>
	struct hash<cgl::Position> {
		size_t operator()(const cgl::Position& p) const;
	};
}

#endif
