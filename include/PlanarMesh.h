#ifndef CGL_PLANARMESH_H
#define CGL_PLANARMESH_H

#include "Mesh.h"

namespace cgl {
	class PlanarMesh : public Mesh {
		public:
			PlanarMesh();
		private:
			static const std::vector<glm::vec3> PLANE_POSITIONS;
			static const std::vector<unsigned int> PLANE_INDICIES;
			static const std::vector<glm::vec3> PLANE_NORMALS;
	};
}

#endif
