#ifndef POSITION_H
#define POSITION_H

#include <functional>

class Position {
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

namespace std {
	template<>
	struct hash<Position> {
		size_t operator()(const Position& p) const;
	};
}

#endif
