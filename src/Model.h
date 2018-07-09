#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <list>
#include <string>
#include "Mesh.h"
#include "Shader.h"
#include "Position.h"
#include "Texture.h"

namespace cgl {
	class Model {
		private:
			std::list<Mesh> meshes;
			void processNode(aiNode* node, const aiScene* scene);
			void processMesh(aiMesh* mesh, const aiScene* scene);
			std::list<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType, type, std::string typeName);
		public:
			Model(const std::string& path);
			void draw(Shader& shader);
	};
}

#endif