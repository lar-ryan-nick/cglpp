#include "PlanarMesh.h"

const std::vector<glm::vec3> cgl::PlanarMesh::PLANE_POSITIONS {
	glm::vec3(-1.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, -1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f)
};
const std::vector<unsigned int> cgl::PlanarMesh::PLANE_INDICIES {0, 1, 2, 1, 2, 3};
const std::vector<glm::vec3> cgl::PlanarMesh::PLANE_NORMALS {
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
};
const std::vector<glm::vec3> cgl::PlanarMesh::PLANE_TANGENTS {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f)
};
const std::vector<glm::vec2> cgl::PlanarMesh::PLANE_TEXCOORDS {
	glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(1.0f, 1.0f)
};

cgl::PlanarMesh::PlanarMesh() : Mesh(PLANE_POSITIONS, PLANE_NORMALS, PLANE_TANGENTS, PLANE_TEXCOORDS, PLANE_INDICIES, std::vector<Mesh::VertexBoneData>(4, {{0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}}), Material()) {}
