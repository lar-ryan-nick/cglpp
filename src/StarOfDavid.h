#ifndef STAROFDAVID_H
#define STAROFDAVID_H

#include <glad/glad.h>

namespace cgl {
	class StarOfDavid {
		private:
			unsigned int VAO;
		public:
			StarOfDavid();
			void bindVAO();
	};
}

#endif