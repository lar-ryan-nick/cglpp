#include "../include/Mesh.h"

cgl::Mesh::Mesh(const std::vector<Position>& p, const std::vector<glm::vec3>& n, const std::vector<Position>& tc, const std::vector<unsigned int>& i, const std::vector<VertexBoneData>& bd, const Material& m) : positions(p), normals(n), textureCoordinates(tc), indicies(i), boneData(bd), material(m) {
	std::vector<glm::vec3> pos(positions.begin(), positions.end());
	std::vector<glm::vec2> texCoord(textureCoordinates.begin(), textureCoordinates.end());
	setupVAO(pos, texCoord);
}

void cgl::Mesh::setupVAO(const std::vector<glm::vec3>& pos, const std::vector<glm::vec2>& texCoord) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(4, vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), &pos[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(glm::vec2), &texCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, boneData.size() * sizeof(VertexBoneData), &boneData[0], GL_STATIC_DRAW);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)sizeof(int[4]));
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);
	// set back to defaults
	glBindVertexArray(0);
}

cgl::Mesh::~Mesh() {
	// buffer data deletion
	glDeleteBuffers(4, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void cgl::Mesh::draw(Shader& shader, const glm::mat4& parentModel) {
	shader.setUniform("material", material);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	// set back to defaults
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

std::vector<cgl::Position> cgl::Mesh::getPositions() {
	return positions;
}
