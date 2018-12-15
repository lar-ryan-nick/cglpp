#ifndef COLOR_H
#define COLOR_H

#include "VectorContainer.h"

namespace cgl {
	class Color : public VectorContainer {
		private:
			void fixValues();
			void fixRed();
			void fixGreen();
			void fixBlue();
			void fixAlpha();
		public:
			Color(float r, float g, float b, float a = 1.0f);
			Color(const glm::vec3 v, float a = 1.0f);
			Color(const glm::vec4 v);
			// functions with same functionality as VectorContainer accessors but make more contextual sense
			float getRed() const;
			float getGreen() const;
			float getBlue() const;
			float getAlpha() const;
			void setRed(float red);
			void setGreen(float green);
			void setBlue(float blue);
			void setAlpha(float alpha);
	};
}

#endif
