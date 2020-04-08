#ifndef CGL_SHADOW_MAP_H
#define CGL_SHADOW_MAP_H

#include "Texture.h"
#include <glad/glad.h>

namespace cgl {
	class ShadowMap {
		public:
			ShadowMap(unsigned int s = 1024);
			~ShadowMap();
			void bindFramebuffer();
			void unbindFramebuffer();
			void bindTexture();
		private:
			unsigned int size;
			unsigned int depthMapFBO;
			Texture depthMap;
	};
}

#endif
