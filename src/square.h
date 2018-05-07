#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <glad/glad.h>

class Square {
	unsigned int VAO;
	public:
		Square();
		void bindVAO();
};

#endif
