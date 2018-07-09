#include "Mesh.h"

cgl::Mesh::Mesh(const std::list<Position>& p, const std::list<glm::vec3>& n, const std::list<Position>& tc, const std::list<unsigned int>& i, const Material& m) : positions(p), normals(n), textureCoordinates(tc), indicies(i), materials(m) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);
	std::vector<glm::vec3> pos(positions.begin(), positions.end());
	std::vector<glm::vec3> norm(normals.begin(), normals.end());
	std::vector<glm::vec2> texCoord(textureCoordinates.begin(), textureCoordinates.end());
	std::vector<unsigned int> ind(indicies.begin(), indicies.end());
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), &pos[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(glm::vec2), &texCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

cgl::Mesh::~Mesh() {
	// buffer data deletion
	glDeleteBuffers(3, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void cgl::Mesh::draw(Shader& shader) {
	// set textures
	unsigned int i = 0;
	std::list<Texture> textureMaps = material.getDiffuseMaps();
	for (std::list<Texture>::iterator it = textureMaps.begin(); it != textureMaps.end(); it++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		ss << "material.diffuseMap[" << i << ']';
		shader.setUniform(ss.str(), i);
		it->bind();
		i++;
	}
	shader.setUniform("material.numDiffuse", textureMaps.size());
	textureMaps = material.getSpecularMaps();
	for (std::list<Texture>::iterator it = textureMaps.begin(); it != textureMaps.end(); it++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		ss << "material.specularMap[" << i << ']';
		shader.setUniform(ss.str(), i);
		it->bind();
		i++;
	}
	shader.setUniform("material.numSpecular", textureMaps.size());
	// draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// set back to defaults
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}