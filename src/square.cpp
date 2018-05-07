#include "square.h"

Square::Square() {
	float verticies[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f,  
		-1.0f, 1.0f, 0.0f, 
	};
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	std::cout << "square2" << std::endl;
}

void Square::bindVAO() {
	glBindVertexArray(VAO);
}
