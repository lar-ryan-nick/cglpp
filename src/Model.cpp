#include "../include/Model.h"

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
	return glm::transpose(glm::make_mat4(&from.a1));
}

cgl::Model::Model(const std::string& path) {
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
	globalInverseTransform = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));
	std::string directory = path.substr(0, path.find_last_of("/") + 1);
	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, directory);

	// let's animate baby!
	startTime = glfwGetTime();
}

cgl::Model::~Model() {
	for (std::list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		delete *it;
	}
}

glm::vec3 cgl::Model::calcInterpolatedTranslation(float time, const aiNodeAnim* nodeAnim) {
	if (nodeAnim->mNumPositionKeys == 1) {
		aiVector3D result = nodeAnim->mPositionKeys[0].mValue;
		return glm::vec3(result.x, result.y, result.z);
	}
	int positionIndex = 0;
	for (int i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) {
		if (time < nodeAnim->mPositionKeys[i + 1].mTime) {
			positionIndex = i;
			break;
		}
	}
	float deltaTime = nodeAnim->mPositionKeys[positionIndex + 1].mTime - nodeAnim->mPositionKeys[positionIndex].mTime;
	float factor = (time - nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
	aiVector3D startPos = nodeAnim->mPositionKeys[positionIndex].mValue;
	glm::vec3 startPosition(startPos.x, startPos.y, startPos.z);
	aiVector3D endPos = nodeAnim->mPositionKeys[positionIndex + 1].mValue;
	glm::vec3 endPosition(endPos.x, endPos.y, endPos.z);
	return glm::lerp(startPosition, endPosition, factor);
}

glm::quat cgl::Model::calcInterpolatedRotation(float time, const aiNodeAnim* nodeAnim) {
	if (nodeAnim->mNumRotationKeys == 1) {
		aiQuaternion result = nodeAnim->mRotationKeys[0].mValue;
		return glm::quat(result.w, result.x, result.y, result.z);
	}
	int rotationIndex = 0;
	for (int i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) {
		if (time < nodeAnim->mRotationKeys[i + 1].mTime) {
			rotationIndex = i;
			break;
		}
	}
	float deltaTime = nodeAnim->mRotationKeys[rotationIndex + 1].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime;
	float factor = (time - nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
	aiQuaternion startQ = nodeAnim->mRotationKeys[rotationIndex].mValue;
	glm::quat startQuat(startQ.w, startQ.x, startQ.y, startQ.z);
	aiQuaternion endQ = nodeAnim->mRotationKeys[rotationIndex + 1].mValue;
	glm::quat endQuat(endQ.w, endQ.x, endQ.y, endQ.z);
	return glm::slerp(startQuat, endQuat, factor);
}

glm::vec3 cgl::Model::calcInterpolatedScaling(float time, const aiNodeAnim* nodeAnim) {
	if (nodeAnim->mNumScalingKeys == 1) {
		aiVector3D result = nodeAnim->mScalingKeys[0].mValue;
		return glm::vec3(result.x, result.y, result.z);
	}
	int scaleIndex = 0;
	for (int i = 0; i < nodeAnim->mNumScalingKeys - 1; i++) {
		if (time < nodeAnim->mScalingKeys[i + 1].mTime) {
			scaleIndex = i;
			break;
		}
	}
	float deltaTime = nodeAnim->mScalingKeys[scaleIndex + 1].mTime - nodeAnim->mScalingKeys[scaleIndex].mTime;
	float factor = (time - nodeAnim->mScalingKeys[scaleIndex].mTime) / deltaTime;
	aiVector3D startS = nodeAnim->mScalingKeys[scaleIndex].mValue;
	glm::vec3 startScale(startS.x, startS.y, startS.z);
	aiVector3D endS = nodeAnim->mScalingKeys[scaleIndex + 1].mValue;
	glm::vec3 endScale(endS.x, endS.y, endS.z);
	return glm::lerp(startScale, endScale, factor);
}

void cgl::Model::updateAnimation(float time, const aiNode* node, const glm::mat4& parentTransform) {
	std::string nodeName(node->mName.C_Str());
	const aiAnimation* animation = importer.GetScene()->mAnimations[0];
	glm::mat4 nodeTransformation(aiMatrix4x4ToGlm(node->mTransformation));
	const aiNodeAnim* nodeAnim = NULL;
	for (int i = 0; i < animation->mNumChannels; i++) {
		if (nodeName == animation->mChannels[i]->mNodeName.C_Str()) {
			nodeAnim = animation->mChannels[i];
			break;
		}
	}
	if (nodeAnim != NULL) {
		glm::vec3 scale = calcInterpolatedScaling(time, nodeAnim);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(), scale);
		glm::quat rotation = calcInterpolatedRotation(time, nodeAnim);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::vec3 translation = calcInterpolatedTranslation(time, nodeAnim);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(), translation);
		nodeTransformation = translationMatrix * rotationMatrix * scaleMatrix;
	}
	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	std::unordered_map<std::string, int>::iterator it = boneMap.find(nodeName);
	if (it != boneMap.end()) {
		bones[it->second].finalTransform = globalInverseTransform * globalTransformation * bones[it->second].offsetTransform;
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		updateAnimation(time, node->mChildren[i], globalTransformation);
	}
}

void cgl::Model::draw(Shader& shader, const glm::mat4& parentModel) {
	const aiScene* scene = importer.GetScene();
	float time = glfwGetTime() - startTime;
	if (scene->mAnimations[0]->mTicksPerSecond == 0) {
		time *= 25.0f;
	} else {
		time *= scene->mAnimations[0]->mTicksPerSecond;
	}
	time = fmod(time, scene->mAnimations[0]->mDuration);
	updateAnimation(time, scene->mRootNode, glm::mat4());
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
	std::vector<unsigned int> indicies;
	// now wak through each of the mesh's faces (a face is a triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}
	// load bones for mesh
	std::vector<Mesh::VertexBoneData> boneData(positions.size());
	for (int i = 0; i < mesh->mNumBones; i++) {
		int boneIndex = 0;
		std::string boneName(mesh->mBones[i]->mName.C_Str());
		std::unordered_map<std::string, int>::iterator it = boneMap.find(boneName);
		if (it == boneMap.end()) {
			boneIndex = bones.size();
			bones.push_back(Bone());
			boneMap.insert(std::pair<std::string, int>(boneName, boneIndex));
		} else {
			boneIndex = it->second;
		}
		bones[boneIndex].offsetTransform = aiMatrix4x4ToGlm(mesh->mBones[i]->mOffsetMatrix);
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
