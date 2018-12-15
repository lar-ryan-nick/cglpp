#ifndef ROTATION_H
#define ROTATION_H

#include "VectorContainer.h"

namespace cgl {
	// TODO: make rotation use quaternion
	class Rotation : public VectorContainer {
		public:
			Rotation(float x = 0.0f, float y = 0.0f, float z = 0.0f);
			void rotate(float x, float y = 0.0f, float z = 0.0f);
	};
}

#endif