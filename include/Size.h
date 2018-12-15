#ifndef SIZE_H
#define SIZE_H

#include "VectorContainer.h"

namespace cgl {
	class Size : public VectorContainer {
	public:
		Size(float x = 1.0f, float y = 1.0f, float z = 1.0f, float w = 1.0f);
		Size(const glm::vec2& v, float z = 1.0f, float w = 1.0f);
		Size(const glm::vec3& v, float w = 1.0f);
		Size(const glm::vec4& v);
		Size(const VectorContainer& vc);
		void scale(float x, float y = 1.0f, float z = 1.0f);
		void scale(const glm::vec2& scalar);
		void scale(const glm::vec3& scalar);
		// functions with same functionality as VectorContainer accessors but make more contextual sense
		float getWidth() const;
		float getHeight() const;
		float getDepth() const;
		void setWidth(float width);
		void setHeight(float height);
		void setDepth(float depth);
	};
}

#endif
