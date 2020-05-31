#ifndef CGL_PIPELINE_H
#define CGL_PIPELINE_H

#include "Size.h"

namespace cgl {
	class Pipeline {
		public:
			Pipeline(const Size& size = Size(1, 1));
			virtual ~Pipeline();
			void bindFramebuffer();
			void bindTexture();
			void updateSize(const Size& size);
		private:
			unsigned int fbo;
			unsigned int texture;
	};
}

#endif
