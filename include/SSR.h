#ifndef CGL_SSR_H
#define CGL_SSR_H

#include "GBuffer.h"
#include "Shader.h"

namespace cgl {
	class SSR {
		public:
			SSR();
			void draw(GBuffer& gBuffer, const glm::mat4& view, const glm::mat4& projection);
			void bindSSR();
		private:
			static Shader* ssrShader;
			static unsigned int vao;
			static unsigned int vbo;
			static unsigned int ebo;
			unsigned int ssrFBO;
			unsigned int ssrBuffer;
	};
}

#endif
