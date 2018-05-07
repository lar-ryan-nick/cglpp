#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glad/glad.h>

class Rectangle {
	float x;
	float y;
	float width;
	float height;
	public:
		Rectangle(float xpos = 0.0f, float ypos = 0.0f, float w = 100.0f, float h = 100.0f);
		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;
		void setX(float xpos);
		void setY(float ypos);
		void setWidth(float w);
		void setHeight(float h);
};

#endif
