#include "starOfDavid.h"

cgl::StarOfDavid::StarOfDavid() {
	float verticies[] = {
		// vertex					 color
		-0.5f, -0.577350269f, 0.5f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.577350269f, 0.5f,   0.0f, 0.0f, 1.0f,
		0.5f, -0.577350269f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.577350269f, -0.5f, 0.0f, 0.0f, 1.0f,

		0.0f, 1.1547005381f, 0.0f,   0.0f, 0.0f, 1.0f,

		-0.5f, 0.577350269f, 0.5f,   0.0f, 0.0f, 1.0f,
		0.5f, 0.577350269f, 0.5f,    0.0f, 0.0f, 1.0f,
		0.5f, 0.577350269f, -0.5f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.577350269f, -0.5f,  0.0f, 0.0f, 1.0f,

		0.0f, -1.1547005381f, 0.0f,   0.0f, 0.0f, 1.0f
	};
	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		0, 3, 4,
		5, 6, 7,
		5, 7, 8,
		5, 6, 9,
		6, 7, 9,
		7, 8, 9,
		5, 8, 9
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void cgl::StarOfDavid::bindVAO() {
	glBindVertexArray(VAO);
}
