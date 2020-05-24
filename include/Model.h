#ifndef CGL_MODEL_H
#define CGL_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include "Animation.h"
#include "Mesh.h"
#include "Shader.h"
#include "Position.h"
#include "Polygon.h"
#include "Texture.h"
#include "Transformable.h"
#include <cassert>
#include <glfw/glfw3.h>

// TODO: calculate center
namespace cgl {
	class Model : public Transformable {
		public:
			// Model takes ownership
			Model(Mesh* mesh);
			Model(const std::string& path);
			~Model();
			std::list<Mesh*> getMeshes();
			void draw(Shader& shader, const glm::mat4& parentModel);
			void applyAnimation(const Animation& animation, float time);
			glm::vec3 getMinBounds() const;
			glm::vec3 getMaxBounds() const;
		private:
			struct Bone {
				std::string name;
				int parent;
				std::vector<int> children;
				glm::mat4 transform;
				glm::mat4 offsetTransform;
				glm::mat4 finalTransform;
			};
			std::unordered_map<std::string, int> boneMap;
			std::vector<Bone> bones;
			int skeletonRoot;
			std::list<Mesh*> meshes;
			glm::mat4 globalInverseTransform;
			void applyAnimation(const Animation& animation, float time, int boneIndex, const glm::mat4& parentTransform);
			void processNode(aiNode* node, const aiScene* scene, const std::string& directory);
			void processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
			void constructSkeleton(aiNode* node, int parentIndex);
	};
}

#endif
