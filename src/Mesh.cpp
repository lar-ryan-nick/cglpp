#include "Mesh.h"

const unsigned int cgl::Mesh::MAX_VERTICIES = 65536;

cgl::Mesh::Mesh(const std::vector<Position>& p, const std::vector<glm::vec3>& n, const std::vector<Position>& tc, const std::list<unsigned int>& i, const Material& m) : positions(p), normals(n), textureCoordinates(tc), indicies(i), material(m) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	std::vector<glm::vec3> pos(positions.size());
	std::vector<glm::vec3> norm(normals.begin(), normals.end());
	std::vector<glm::vec2> texCoord(textureCoordinates.begin(), textureCoordinates.end());
	std::vector<unsigned int> ind(indicies.begin(), indicies.end());
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), viewport[2] / viewport[3], 0.1f, 1000.0f);
	for (int i = 0; i < positions.size(); i++) {
		pos[i] = projection * model * positions[i];
		//if (abs(pos[i].x) < 1 && abs(pos[i].y) < 1 && abs(pos[i].z) < 1) {
			//std::cout << Position(pos[i]) << std::endl;
		//}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), &pos[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(glm::vec2), &texCoord[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(glm::vec2), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_VERTICIES * sizeof(unsigned int), NULL, GL_STREAM_DRAW);
	glBindVertexArray(0);
}

cgl::Mesh::~Mesh() {
	// buffer data deletion
	glDeleteBuffers(3, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void cgl::Mesh::draw(Shader& shader, const glm::mat4& mvp, const Polygon& clippingRegion) {
	shader.setUniform("material", material);
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> texCoord;
	std::vector<unsigned int> ind;
	/*
	for (std::list<unsigned int>::iterator it = indicies.begin(); it != indicies.end(); it++) {
		unsigned int index1 = *it;
		it++;
		unsigned int index2 = *it;
		it++;
		unsigned int index3 = *it;
		Polygon face;
		face.addVertex(static_cast<Position>(mvp * positions[index1]));
		face.addVertex(static_cast<Position>(mvp * positions[index2]));
		face.addVertex(static_cast<Position>(mvp * positions[index3]));
		for (std::list<Position>::iterator it2 = face.getVerticies().begin(); it2 != face.getVerticies().end(); it2++) {
			it2->setW(1.0f);
			//std::cout << *it2 << std::endl;
		}
		std::list<Polygon> clippedPolygons = face.clipTo(clippingRegion);
		for (std::list<Polygon>::iterator it2 = clippedPolygons.begin(); it2 != clippedPolygons.end(); it2++) {
			std::list<Position> verticies = it2->getVerticies();
			if (verticies.size() < 3) {
				return;
			}
			for (int i = 0; i < (verticies.size() - 2) * 3; i += 3) {
				ind.push_back(pos.size());
				ind.push_back(pos.size() + i / 3 + 1);
				ind.push_back(pos.size() + i / 3 + 2);
			}
			pos.insert(pos.end(), verticies.begin(), verticies.end());
			for (int i = 0; i < verticies.size(); i++) {
				norm.push_back(normals[index1]);
				texCoord.push_back(textureCoordinates[index1]); //TODO: find equation for transformation matrix
			}
		}
	}
	*/
	pos = std::vector<glm::vec3>(positions.size());
	norm = std::vector<glm::vec3>(normals.begin(), normals.end());
	texCoord = std::vector<glm::vec2>(textureCoordinates.begin(), textureCoordinates.end());
	ind = std::vector<unsigned int>(indicies.begin(), indicies.end());
	for (int i = 0; i < positions.size(); i++) {
		pos[i] = mvp * positions[i];
	}
	if (pos.size() > 0) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, pos.size() * sizeof(glm::vec3), &pos[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, norm.size() * sizeof(glm::vec3), &norm[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, texCoord.size() * sizeof(glm::vec2), &texCoord[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ind.size() * sizeof(unsigned int), &ind[0]);
		glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	}
	// set back to defaults
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
