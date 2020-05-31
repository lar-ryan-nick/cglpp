#include "VAO.h"
#include <glad/glad.h>

cgl::VAO* cgl::VAO::screenVAO = nullptr;

cgl::VAO* cgl::VAO::getScreenVAO() {
	if (screenVAO == nullptr) {
		screenVAO = new VAO({
				glm::vec2(-1.0f, -1.0f),
				glm::vec2(-1.0f, 1.0f),
				glm::vec2(1.0f, -1.0f),
				glm::vec2(1.0f, 1.0f)
			}, {0, 1, 2, 1, 2, 3});
	}
	return screenVAO;
}

cgl::VAO::VAO(const std::vector<glm::vec2>& verticies, const std::vector<unsigned int>& indicies) : numIndicies(indicies.size()) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * verticies.size(), &verticies[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), &indicies[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cgl::VAO::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
