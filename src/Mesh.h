#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <list>
#include <cmath>
#include <vector>
#include <string>
#include "Position.h"
#include "Polygon.h"
#include "Rotation.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"

namespace cgl {
	class Mesh {
		private:
			const static unsigned int MAX_VERTICIES;
			unsigned int vbo[3];
			unsigned int ebo;
			unsigned int vao;
			std::vector<Position> positions;
			std::vector<glm::vec3> normals;
			std::vector<Position> textureCoordinates;
			std::list<unsigned int> indicies;
			Material material;
		public:
			Mesh(const std::vector<Position>& p, const std::vector<glm::vec3>& n, const std::vector<Position>& tc, const std::list<unsigned int>& i, const Material& m);
			~Mesh();
			void draw(Shader& shader, const glm::mat4& mvp, const Polygon& clippingRegion);
	};
}

#endif
