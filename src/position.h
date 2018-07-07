#ifndef POSITION_H
#define POSITION_H

#include "VectorContainer.h"

namespace cgl {
	class Position : public VectorContainer {
		public:
			Position(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
			void move(float x, float y = 0.0f, float z = 0.0f);
	};
}

#endif
