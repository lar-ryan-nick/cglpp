#ifndef SIZE_H
#define SIZE_H

namespace cgl {
	class Size {
		private:
			float width;
			float height;
		public:
			Size(float w = 0, float h = 0);
			float getWidth() const;
			float getHeight() const;
			void setWidth(float w);
			void setHeight(float h);
	};
}

#endif
