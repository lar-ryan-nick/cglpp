#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>

namespace cgl {
	class Cube {
		private:
			unsigned int VAO;
		public:
			Cube();
			void bindVAO();
	};
}

#endif
