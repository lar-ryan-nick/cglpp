#ifndef POSITION_H
#define POSITION_H

#include "VectorContainer.h"

namespace cgl {
	class Position : public VectorContainer {
		public:
			Position(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
			Position(const glm::vec2& v, float z = 0.0f, float w = 1.0f);
			Position(const glm::vec3& v, float w = 1.0f);
			Position(const glm::vec4& v);
			Position(const VectorContainer& vc);
			void move(float x, float y = 0.0f, float z = 0.0f);
	};
}

#endif
