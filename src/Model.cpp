#include "Model.h"

cgl::Model::Model(const std::string& path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		std::cerr << path << std::endl;
		return;
	}
	std::string directory = path.substr(0, path.find_last_of("/") + 1);
	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, directory);
}

void cgl::Model::draw(Shader& shader) {
	for (std::list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		it->draw(shader);
	}
}

void cgl::Model::processNode(aiNode* node, const aiScene* scene, const std::string& directory) {
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, directory);
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, directory);
	}
}

void cgl::Model::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory) {
	std::list<Position> positions;
	std::list<glm::vec3> normals;
	std::list<Position> textureCoordinates;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		positions.push_back(Position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		Position textureCoordinate;
		if (mesh->mTextureCoords[0]) {
			textureCoordinate = Position(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		textureCoordinates.push_back(textureCoordinate);
	}
	std::list<unsigned int> indicies;
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}
	aiMaterial* m = scene->mMaterials[mesh->mMaterialIndex];
	std::list<Texture> diffuseMaps = loadMaterialTextures(m, aiTextureType_DIFFUSE, directory);
	std::list<Texture> specularMaps = loadMaterialTextures(m, aiTextureType_SPECULAR, directory);
	float shininess = 0;
	m->Get(AI_MATKEY_SHININESS, shininess);
	Material material(diffuseMaps, specularMaps, shininess);
	meshes.push_back(Mesh(positions, normals, textureCoordinates, indicies, material));
}

std::list<cgl::Texture> cgl::Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& directory) {
	std::list<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		std::cout << type << str.C_Str() << std::endl;
		textures.push_back(Texture(directory + str.data));
	}
	return textures;
}