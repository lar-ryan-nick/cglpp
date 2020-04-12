#ifndef CGL_CASCADED_SHADOW_MAP_H
#define CGL_CASCADED_SHADOW_MAP_H

#include <glad/glad.h>
#include <limits>
#include "Texture.h"
#include "DirectionalLight.h"
#include "Camera.h"

namespace cgl {
	class CascadedShadowMap {
		public:
			constexpr static unsigned int MAX_CASCADES = 8;
			CascadedShadowMap(int nc = 4, unsigned int size = 1024);
			~CascadedShadowMap();
			int getNumCascades() const;
			void bindFramebuffer(int cascadeNum);
			void bindShadowMapArray();
			float getCascadeEnd(int cascadeNum) const;
			glm::mat4 getLightViewProjection(int cascadeNum) const;
			void updateSplits(float zNear, float zFar);
			/**
			 * @param yfov ithe vertical field of view. Same as that passed to glm::ortho
			 * @param aspectRatio the aspect ratio of the camera in terms of width over height
			 */
			void updateLightViewProjections(const Camera& camera, const DirectionalLight& directionalLight, float yfov, float aspectRatio);
		private:
			unsigned int shadowMapSize;
			unsigned int fbo;
			int numCascades;
			unsigned int shadowMapArray;
			float cascadeSplits[MAX_CASCADES];
			glm::mat4 lightViewProjections[MAX_CASCADES];
	};
}

#endif
