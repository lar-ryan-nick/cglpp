#ifndef CGL_MESH_H
#define CGL_MESH_H

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
#include "Transformable.h"

// TODO: Calculate center
namespace cgl {
	class SkeletalMesh : public Transformable {
		public:
			struct VertexBoneData {
				int indicies[4];
				float weights[4];
			};
			SkeletalMesh(const std::vector<glm::vec3>& p, const std::vector<glm::vec3>& n, const std::vector<glm::vec2>& tc, const std::vector<unsigned int>& i, const std::vector<VertexBoneData>& bd, const Material& m);
			~SkeletalMesh();
			void draw(Shader& shader, const glm::mat4& parentModel);
			std::vector<glm::vec3> getPositions();
			glm::vec3 getMinBounds() const;
			glm::vec3 getMaxBounds() const;
		private:
			unsigned int vao;
			unsigned int vbo[4];
			unsigned int ebo;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> textureCoordinates;
			std::vector<unsigned int> indicies;
			std::vector<VertexBoneData> boneData;
			Material material;
			glm::vec3 minBounds;
			glm::vec3 maxBounds;
			void setupVAO();
			void setupBounds();
	};
}

#endif
