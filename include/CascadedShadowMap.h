#ifndef CGL_CASCADED_SHADOW_MAP_H
#define CGL_CASCADED_SHADOW_MAP_H

#include <glad/glad.h>
#include "Texture.h"

namespace cgl {
	class CascadedShadowMap {
		public:
			CascadedShadowMap(int nc = 3, int size = 1024);
			~CascadedShadowMap();
			void bindFramebuffer(int cascadeNum);
			Texture& getShadowMap(int cascadeNum);
		private:
			unsigned int shadowMapSize;
			unsigned int fbo;
			int numCascades
			Texture* shadowMaps;
	};
}

#endif
