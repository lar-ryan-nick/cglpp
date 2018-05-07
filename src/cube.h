#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>

class Cube {
	unsigned int VAO;
	public:
		Cube();
		void bindVAO();
};

#endif
