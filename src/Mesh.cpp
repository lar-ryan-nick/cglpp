#include "Mesh.h"

cgl::Mesh::Mesh(const std::vector<glm::vec3>& p, const std::vector<glm::vec3>& n, const std::vector<glm::vec3>& t, const std::vector<glm::vec2>& tc, const std::vector<unsigned int>& i, const std::vector<VertexBoneData>& bd, const Material& m) : positions(p), normals(n), tangents(t), textureCoordinates(tc), indicies(i), boneData(bd), material(m), minBounds(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), maxBounds(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()) {
	setupVAO();
	setupBounds();
}

void cgl::Mesh::setupVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);
	glGenBuffers(5, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(glm::vec2), &textureCoordinates[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, boneData.size() * sizeof(VertexBoneData), &boneData[0], GL_STATIC_DRAW);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)sizeof(int[4]));
	glEnableVertexAttribArray(5);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cgl::Mesh::setupBounds() {
	for (int i = 0; i < positions.size(); ++i) {
		minBounds.x = glm::min(minBounds.x, positions[i].x);
		maxBounds.x = glm::max(maxBounds.x, positions[i].x);
		minBounds.y = glm::min(minBounds.y, positions[i].y);
		maxBounds.y = glm::max(maxBounds.y, positions[i].y);
		minBounds.z = glm::min(minBounds.z, positions[i].z);
		maxBounds.z = glm::max(maxBounds.z, positions[i].z);
	}
}

cgl::Mesh::~Mesh() {
	// buffer data deletion
	glDeleteBuffers(5, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void cgl::Mesh::draw(Shader& shader, const glm::mat4& parentModel) {
	glm::mat4 model = parentModel * getTransformationMatrix();
	shader.setUniform("model", model);
	shader.setUniform("material", material);
	/*
	if (indicies.size() == 6) {
		for (int i = 0; i < indicies.size(); ++i) {
			std::cout << Position(model * glm::vec4(positions[indicies[i]], 1.0f)) << std::endl;
		}
		std::cout << std::endl;
	}
	*/
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<glm::vec3> cgl::Mesh::getPositions() {
	return positions;
}

cgl::Material& cgl::Mesh::getMaterial() {
	return material;
}

const cgl::Material& cgl::Mesh::getMaterial() const {
	return material;
}

glm::vec3 cgl::Mesh::getMinBounds() const {
	return minBounds;
}

glm::vec3 cgl::Mesh::getMaxBounds() const {
	return maxBounds;
}
