#ifndef POSITION_H
#define POSITION_H

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
};

#endif
