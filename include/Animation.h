#ifndef ANIMATION_H
#define ANIMATION_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cgl {
	class Animation {
		private:
			class NodeAnimation {
				private:
					struct VectorKey {
						glm::vec3 vector;
						float time;
						static VectorKey VectorKeyFromAssimp(const aiVectorKey& vk);
					};
					struct QuaternionKey {
						glm::quat quaternion;
						float time;
						static QuaternionKey QuaternionKeyFromAssimp(const aiQuatKey& qk);
					};
					std::vector<VectorKey> translationKeys;
					std::vector<VectorKey> scaleKeys;
					std::vector<QuaternionKey> rotationKeys;
				public:
					static NodeAnimation NodeAnimationFromAssimp(aiNodeAnim* nodeAnim);
					glm::vec3 getInterpolatedTranslation(float time) const;
					glm::vec3 getInterpolatedScale(float time) const;
					glm::quat getInterpolatedRotation(float time) const;
					glm::mat4 getTransformation(float time) const;
			};
			float ticksPerSecond;
			float duration;
			std::unordered_map<std::string, NodeAnimation> nodeAnimations;
		public:
			Animation(const std::string& path);
			float getTicksPerSecond() const;
			float getDuration() const;
			bool getTransformation(const std::string& nodeName, float time, glm::mat4& transformation) const;
	};
}

#endif
