#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
		public:
			struct VertexBoneData {
				int indicies[4];
				float weights[4];
			};
		private:
			unsigned int vao;
			unsigned int vbo[4];
			unsigned int ebo;
			std::vector<Position> positions;
			std::vector<glm::vec3> normals;
			std::vector<Position> textureCoordinates;
			std::vector<VertexBoneData> boneData;
			std::vector<unsigned int> indicies;
			Material material;
			void setupVAO(const std::vector<glm::vec3>& pos, const std::vector<glm::vec2>& texCoord);
		public:
			Mesh(const std::vector<Position>& p, const std::vector<glm::vec3>& n, const std::vector<Position>& tc, const std::vector<unsigned int>& i, const std::vector<VertexBoneData>& bd, const Material& m);
			~Mesh();
			void draw(Shader& shader, const glm::mat4& parentModel);
			std::vector<Position> getPositions();
	};
}

#endif
