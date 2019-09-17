#include "../include/Model.h"

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
	return glm::transpose(glm::make_mat4(&from.a1));
}

cgl::Model::Model(const std::string& path) : skeletonRoot(-1) {
	Assimp::Importer importer;
	// read file via ASSIMP
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_LIGHTS | aiComponent_CAMERAS);
	const aiScene* scene = importer.ReadFile(path, aiProcess_GenUVCoords | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_SplitLargeMeshes | aiProcess_SortByPType | aiProcess_FindDegenerates | aiProcess_LimitBoneWeights/* | aiProcess_CalcTangentSpace*/ | aiProcess_ImproveCacheLocality | aiProcess_RemoveComponent | aiProcess_Triangulate);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		std::cerr << path << std::endl;
		return;
	}
	//globalInverseTransform = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));
	std::string directory = path.substr(0, path.find_last_of("/") + 1);
	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, directory);
	constructSkeleton(scene->mRootNode, -1);
	// let's animate baby!
	animation = Animation::AnimationFromAssimp(scene->mAnimations[0]);

	startTime = glfwGetTime();
}

cgl::Model::~Model() {
	for (std::list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		delete *it;
	}
}

void cgl::Model::constructSkeleton(aiNode* node, int parentIndex) {
	std::unordered_map<std::string, int>::iterator it = boneMap.find(std::string(node->mName.C_Str()));
	if (it != boneMap.end()) {
		bones[it->second].parent = parentIndex;
		if (bones[it->second].parent < 0) {
			skeletonRoot = it->second;
		}
		bones[it->second].transform = aiMatrix4x4ToGlm(node->mTransformation);
		for (int i = 0; i < node->mNumChildren; i++) {
			std::unordered_map<std::string, int>::iterator finder = boneMap.find(std::string(node->mChildren[i]->mName.C_Str()));
			if (finder != boneMap.end()) {
				bones[it->second].children.push_back(finder->second);
				constructSkeleton(node->mChildren[i], it->second);
			}
		}
	} else if (parentIndex < 0) {
		for (int i = 0; i < node->mNumChildren; i++) {
			constructSkeleton(node->mChildren[i], parentIndex);
		}
	}
}

void cgl::Model::updateAnimation(float time, int boneIndex, const glm::mat4& parentTransform) {
	glm::mat4 nodeTransformation;
	if (!animation.getTransformation(bones[boneIndex].name, time, nodeTransformation)) {
		nodeTransformation = bones[boneIndex].transform;
	}
	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	bones[boneIndex].finalTransform = /*globalInverseTransform * */globalTransformation * bones[boneIndex].offsetTransform;
	for (int i = 0; i < bones[boneIndex].children.size(); i++) {
		updateAnimation(time, bones[boneIndex].children[i], globalTransformation);
	}
}

void cgl::Model::draw(Shader& shader, const glm::mat4& parentModel) {
	float time = glfwGetTime() - startTime;
	time *= animation.getTicksPerSecond();
	time = fmod(time, animation.getDuration());
	updateAnimation(time, skeletonRoot, glm::mat4());
	for (int i = 0; i < bones.size(); i++) {
		std::stringstream ss;
		ss << "boneTransforms[" << i << "]";
		shader.setUniform(ss.str(), bones[i].finalTransform);
	}
	for (std::list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		Mesh* m = *it;
		m->draw(shader, parentModel);
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
	std::vector<Position> positions;
	std::vector<glm::vec3> normals;
	std::vector<Position> textureCoordinates;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		positions.push_back(Position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		Position textureCoordinate;
		// TODO: figure out how to support up to 8 different texture coordinates
		// psuedocode is in documentation
		if (mesh->mTextureCoords[0]) {
			textureCoordinate = Position(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		textureCoordinates.push_back(textureCoordinate);
	}
	// now wak through each of the mesh's faces (a face is a triangle) and retrieve the corresponding vertex indices.
	std::vector<unsigned int> indicies(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indicies[3 * i + j] = face.mIndices[j];
		}
	}
	// load bones for mesh
	std::vector<Mesh::VertexBoneData> boneData(positions.size());
	for (int i = 0; i < mesh->mNumBones; i++) {
		std::string boneName(mesh->mBones[i]->mName.C_Str());
		int boneIndex = 0;
		std::unordered_map<std::string, int>::iterator it = boneMap.find(boneName);
		if (it == boneMap.end()) {
			boneIndex = bones.size();
			bones.push_back(Bone());
			boneMap.insert(std::pair<std::string, int>(boneName, boneIndex));
		} else {
			boneIndex = it->second;
		}
		bones[boneIndex].name = mesh->mBones[i]->mName.C_Str();
		bones[boneIndex].offsetTransform = aiMatrix4x4ToGlm(mesh->mBones[i]->mOffsetMatrix);
		aiNode* parentBoneNode = scene->mRootNode->FindNode(mesh->mBones[i]->mName)->mParent;
		int currentBoneIndex = boneIndex;
		while (parentBoneNode != NULL && parentBoneNode->mNumChildren > 1) {
			std::string parentBoneName(parentBoneNode->mName.C_Str());
			std::unordered_map<std::string, int>::iterator parentIt = boneMap.find(parentBoneName);
			if (it == boneMap.end()) {
				bones[currentBoneIndex].parent = bones.size();
				bones.push_back(Bone());
				boneMap.insert(std::pair<std::string, int>(parentBoneName, bones[currentBoneIndex].parent));
			} else {
				bones[currentBoneIndex].parent = it->second;
				break;
			}
			currentBoneIndex = bones[currentBoneIndex].parent;
			parentBoneNode = parentBoneNode->mParent;
		}
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			int index = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			for (int k = 0; k < 4; k++) {
				if (boneData[index].weights[k] == 0.0f) {
					boneData[index].indicies[k] = boneIndex;
					boneData[index].weights[k] = weight;
					break;
				}
			}
		}
	}
	aiMaterial* m = scene->mMaterials[mesh->mMaterialIndex];
	meshes.push_back(new Mesh(positions, normals, textureCoordinates, indicies, boneData, Material::materialFromAssimp(m, scene, directory)));
}

std::list<cgl::Mesh*> cgl::Model::getMeshes() {
	return meshes;
}
