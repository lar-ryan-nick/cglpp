#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <string>
#include "Position.h"
#include "Rotation.h"
#include "Material.h"
#include "Shader.h"

namespace cgl {
	class Mesh {
		private:
			unsigned int vbo[3];
			unsigned int ebo;
			unsigned int vao;
			std::list<Position> positions;
			std::list<glm::vec3> normals;
			std::list<Position> textureCoordinates;
			std::list<unsigned int> indicies;
			Material material;
		public:
			Mesh(const std::list<Position>& p, const std::list<glm::vec3>& n, const std::list<Position>& tc, const std::list<unsigned int>& i, const Material& m);
			~Mesh();
			void draw(Shader& shader);
	};
}

#endif
