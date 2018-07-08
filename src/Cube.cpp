#include "Cube.h"

cgl::Cube::Cube() {
	float verticies[] = {
		// vertex			 color			   tex coord   normal
		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,

		0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f
	};
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		5, 6, 7,
		8, 9, 10,
		9, 10, 11,
		12, 13, 14,
		13, 14, 15,
		16, 17, 18,
		17, 18, 19,
		20, 21, 22,
		20, 22, 23
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
}

void cgl::Cube::bindVAO() {
	glBindVertexArray(VAO);
}
