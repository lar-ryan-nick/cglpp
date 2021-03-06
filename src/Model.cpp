#include "Model.h"

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
	return glm::transpose(glm::make_mat4(&from.a1));
}

cgl::Model::Model(Mesh* mesh) : skeletonRoot(-1), globalInverseTransform(1.0f) {
	meshes.push_back(mesh);
}

cgl::Model::Model(const std::string& path) : skeletonRoot(-1) {
	Assimp::Importer importer;
	// read file via ASSIMP
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_ANIMATIONS | aiComponent_COLORS | aiComponent_NORMALS | aiComponent_TANGENTS_AND_BITANGENTS);
	const aiScene* scene = importer.ReadFile(path, aiProcess_GenUVCoords | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_SplitLargeMeshes | aiProcess_SortByPType | aiProcess_FindDegenerates | aiProcess_LimitBoneWeights | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality | aiProcess_RemoveComponent | aiProcess_Triangulate | aiProcess_ValidateDataStructure | aiProcess_RemoveRedundantMaterials | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		std::cerr << path << std::endl;
		return;
	}
	globalInverseTransform = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));
	std::string directory = path.substr(0, path.find_last_of("/") + 1);
	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, directory);
	constructSkeleton(scene->mRootNode, -1);
	importer.FreeScene();
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

void cgl::Model::applyAnimation(const Animation& animation, float time) {
	applyAnimation(animation, time, skeletonRoot, glm::mat4(1.0f));
}

void cgl::Model::applyAnimation(const Animation& animation, float time, int boneIndex, const glm::mat4& parentTransform) {
	glm::mat4 nodeTransformation(1.0);
	if (!animation.getTransformation(bones[boneIndex].name, time, nodeTransformation)) {
		nodeTransformation = bones[boneIndex].transform;
	}
	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	bones[boneIndex].finalTransform = /*globalInverseTransform * */globalTransformation * bones[boneIndex].offsetTransform;
	for (int i = 0; i < bones[boneIndex].children.size(); i++) {
		applyAnimation(animation, time, bones[boneIndex].children[i], globalTransformation);
	}
}

void cgl::Model::draw(Shader& shader, const glm::mat4& parentModel) {
	for (int i = 0; i < bones.size(); i++) {
		std::stringstream ss;
		ss << "boneTransforms[" << i << "]";
		shader.setUniform(ss.str(), bones[i].finalTransform);
	}
	if (bones.empty()) {
		shader.setUniform("boneTransforms[0]", glm::mat4(1.0f));
	}
	glm::mat4 model = parentModel * getTransformationMatrix();
	for (std::list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		Mesh* m = *it;
		m->draw(shader, model);
	}
}

glm::vec3 cgl::Model::getMinBounds() const {
	glm::vec3 minBound(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	for (std::list<Mesh*>::const_iterator it = meshes.cbegin(); it != meshes.cend(); it++) {
		Mesh* m = *it;
		glm::vec3 mb = m->getMinBounds();
		minBound.x = glm::min(minBound.x, mb.x);
		minBound.y = glm::min(minBound.y, mb.y);
		minBound.z = glm::min(minBound.z, mb.z);
	}
	return minBound;
}

glm::vec3 cgl::Model::getMaxBounds() const {
	glm::vec3 maxBound(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
	for (std::list<Mesh*>::const_iterator it = meshes.cbegin(); it != meshes.cend(); it++) {
		Mesh* m = *it;
		glm::vec3 mb = m->getMaxBounds();
		maxBound.x = glm::max(maxBound.x, mb.x);
		maxBound.y = glm::max(maxBound.y, mb.y);
		maxBound.z = glm::max(maxBound.z, mb.z);
	}
	return maxBound;
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
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> textureCoordinates;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		tangents.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
		glm::vec2 textureCoordinate;
		// TODO: figure out how to support up to 8 different texture coordinates
		// psuedocode is in documentation
		if (mesh->mTextureCoords[0]) {
			textureCoordinate = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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
	std::vector<Mesh::VertexBoneData> boneData(positions.size(), {{0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}});
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
			if (parentIt == boneMap.end()) {
				bones[currentBoneIndex].parent = bones.size();
				bones.push_back(Bone());
				boneMap.insert(std::pair<std::string, int>(parentBoneName, bones[currentBoneIndex].parent));
			} else {
				bones[currentBoneIndex].parent = parentIt->second;
				break;
			}
			currentBoneIndex = bones[currentBoneIndex].parent;
			parentBoneNode = parentBoneNode->mParent;
		}
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			int index = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			for (int k = 0; k < 4; k++) {
				if (boneData[index].weights[k] == 0.0f || (k == 0 && boneData[index].weights[k] == 1.0f)) {
					boneData[index].indicies[k] = boneIndex;
					boneData[index].weights[k] = weight;
					break;
				}
			}
		}
	}
	/* Hopefully not needed with constructor in vector constructor
	for (int i = 0; i < boneData.size(); ++i) {
		if (boneData[i].weights[0] == 0.0f) {
			boneData[i].weights[0] = 1.0f;
			for (int j = 0; j < 4; ++j) {
				boneData[i].indicies[j] = 0;
			}
		}
	}
	*/
	aiMaterial* m = scene->mMaterials[mesh->mMaterialIndex];
	meshes.push_back(new Mesh(positions, normals, tangents, textureCoordinates, indicies, boneData, Material::materialFromAssimp(m, scene, directory)));
}

std::list<cgl::Mesh*> cgl::Model::getMeshes() {
	return meshes;
}
