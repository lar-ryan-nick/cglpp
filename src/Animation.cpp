#include "Animation.h"

cgl::Animation::NodeAnimation::VectorKey cgl::Animation::NodeAnimation::VectorKey::VectorKeyFromAssimp(const aiVectorKey& vk) {
	VectorKey vectorKey;
	vectorKey.vector = glm::vec3(vk.mValue.x, vk.mValue.y, vk.mValue.z);
	vectorKey.time = vk.mTime;
	return vectorKey;
}

cgl::Animation::NodeAnimation::QuaternionKey cgl::Animation::NodeAnimation::QuaternionKey::QuaternionKeyFromAssimp(const aiQuatKey& qk) {
	QuaternionKey quaternionKey;
	quaternionKey.quaternion = glm::quat(qk.mValue.w, qk.mValue.x, qk.mValue.y, qk.mValue.z);
	quaternionKey.time = qk.mTime;
	return quaternionKey;
}

cgl::Animation::NodeAnimation cgl::Animation::NodeAnimation::NodeAnimationFromAssimp(aiNodeAnim* nodeAnim) {
	NodeAnimation nodeAnimation;
	nodeAnimation.translationKeys.resize(nodeAnim->mNumPositionKeys);
	for (int i = 0; i < nodeAnim->mNumPositionKeys; i++) {
		nodeAnimation.translationKeys[i] = VectorKey::VectorKeyFromAssimp(nodeAnim->mPositionKeys[i]);
	}
	nodeAnimation.scaleKeys.resize(nodeAnim->mNumScalingKeys);
	for (int i = 0; i < nodeAnim->mNumScalingKeys; i++) {
		nodeAnimation.scaleKeys[i] = VectorKey::VectorKeyFromAssimp(nodeAnim->mScalingKeys[i]);
	}
	nodeAnimation.rotationKeys.resize(nodeAnim->mNumRotationKeys);
	for (int i = 0; i < nodeAnim->mNumRotationKeys; i++) {
		nodeAnimation.rotationKeys[i] = QuaternionKey::QuaternionKeyFromAssimp(nodeAnim->mRotationKeys[i]);
	}
	return nodeAnimation;
}

glm::vec3 cgl::Animation::NodeAnimation::getInterpolatedTranslation(float time) const {
	if (translationKeys.size() < 2) {
		return translationKeys[0].vector;
	}
	int index = 0;
	for (int i = 0; i < translationKeys.size() - 1; i++) {
		if (time < translationKeys[i + 1].time) {
			index = i;
			break;
		}
	}
	float deltaTime = translationKeys[index + 1].time - translationKeys[index].time;
	float factor = (time - translationKeys[index].time) / deltaTime;
	return glm::lerp(translationKeys[index].vector, translationKeys[index + 1].vector, factor);
}

glm::vec3 cgl::Animation::NodeAnimation::getInterpolatedScale(float time) const {
	if (scaleKeys.size() < 2) {
		return scaleKeys[0].vector;
	}
	int index = 0;
	for (int i = 0; i < scaleKeys.size() - 1; i++) {
		if (time < scaleKeys[i + 1].time) {
			index = i;
			break;
		}
	}
	float deltaTime = scaleKeys[index + 1].time - scaleKeys[index].time;
	float factor = (time - scaleKeys[index].time) / deltaTime;
	return glm::lerp(scaleKeys[index].vector, scaleKeys[index + 1].vector, factor);
}

glm::quat cgl::Animation::NodeAnimation::getInterpolatedRotation(float time) const {
	if (rotationKeys.size() < 2) {
		return rotationKeys[0].quaternion;
	}
	int index = 0;
	for (int i = 0; i < rotationKeys.size() - 1; i++) {
		if (time < rotationKeys[i + 1].time) {
			index = i;
			break;
		}
	}
	float deltaTime = rotationKeys[index + 1].time - rotationKeys[index].time;
	float factor = (time - rotationKeys[index].time) / deltaTime;
	return glm::slerp(rotationKeys[index].quaternion, rotationKeys[index + 1].quaternion, factor);
}

glm::mat4 cgl::Animation::NodeAnimation::getTransformation(float time) const {
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), getInterpolatedScale(time));
	glm::mat4 rotationMatrix = glm::mat4_cast(getInterpolatedRotation(time));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), getInterpolatedTranslation(time));
	return translationMatrix * rotationMatrix * scaleMatrix;
}

cgl::Animation::Animation(const std::string& path) : ticksPerSecond(0.0f), duration(0.0f) {
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_NORMALS | aiComponent_TANGENTS_AND_BITANGENTS | aiComponent_COLORS | aiComponent_TEXCOORDS | aiComponent_BONEWEIGHTS | aiComponent_TEXTURES | aiComponent_MESHES | aiComponent_MATERIALS);
	const aiScene* scene = importer.ReadFile(path, aiProcess_OptimizeGraph | aiProcess_RemoveComponent | aiProcess_ValidateDataStructure);
	if (scene == nullptr) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		std::cerr << path << std::endl;
		return;
	}
	if (!scene->HasAnimations()) {
		std::cerr << path << " has no animations" << std::endl;
		return;
	}
	// TODO: make helper function to combine logic between this and the static constructor
	aiAnimation* anim = scene->mAnimations[0];
	ticksPerSecond = anim->mTicksPerSecond == 0.0f ? 25.0f : anim->mTicksPerSecond;
	duration = anim->mDuration;
	for (int i = 0; i < anim->mNumChannels; i++) {
		nodeAnimations.insert(std::pair<std::string, NodeAnimation>(anim->mChannels[i]->mNodeName.C_Str(), NodeAnimation::NodeAnimationFromAssimp(anim->mChannels[i])));
	}
}

float cgl::Animation::getTicksPerSecond() const {
	return ticksPerSecond;
}

float cgl::Animation::getDuration() const {
	return duration;
}

bool cgl::Animation::getTransformation(const std::string& nodeName, float time, glm::mat4& transformation) const {
	std::unordered_map<std::string, NodeAnimation>::const_iterator it = nodeAnimations.find(nodeName);
	if (it == nodeAnimations.end()) {
		return false;
	}
	transformation = it->second.getTransformation(time);
	return true;
}
