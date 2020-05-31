#ifndef CGL_GBUFFER_H
#define CGL_GBUFFER_H

#include "Texture.h"

namespace cgl {
	class GBuffer {
		public:
			GBuffer();
			void bindBuffer();
			void bindPositionTexture();
			void bindNormalTexture();
			//void bindTangentTexture();
			void bindAmbientTexture();
			void bindDiffuseTexture();
			void bindSpecularTexture();
		private:
			unsigned int fbo;
			unsigned int positionBuffer;
			unsigned int normalBuffer;
			//unsigned int tangentBuffer;
			unsigned int ambientBuffer;
			unsigned int diffuseBuffer;
			unsigned int specularBuffer;
	};
}

#endif
