#ifndef CGL_VAO_H
#define CGL_VAO_H

#include <glm/glm.hpp>
#include <vector>

namespace cgl {
	class VAO {
		public:
			static VAO* getScreenVAO();
			void draw();
			// TODO: Add good constructor and destructor mechanisms
		private:
			VAO(const std::vector<glm::vec2>& verticies, const std::vector<unsigned int>& indicies);
			static VAO* screenVAO;
			unsigned int vao;
			unsigned int vbo;
			unsigned int ebo;
			unsigned int numIndicies;
	};
}

#endif
