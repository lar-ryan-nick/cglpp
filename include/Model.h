#ifndef MODEL_H
#define MODEL_H

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
#include <cassert>
#include <glfw/glfw3.h>

namespace cgl {
	class Model {
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
			Animation animation;
			float startTime;
			void updateAnimation(float time, int boneIndex, const glm::mat4& parentTransform);
			void processNode(aiNode* node, const aiScene* scene, const std::string& directory);
			void processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
			void constructSkeleton(aiNode* node, int parentIndex);
		public:
			Model(const std::string& path);
			~Model();
			void draw(Shader& shader, const glm::mat4& parentModel);
			std::list<Mesh*> getMeshes();
	};
}

#endif
