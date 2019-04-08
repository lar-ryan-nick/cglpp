#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <string>
#include "View.h"
#include "Texture.h"
#include "Shader.h"

namespace cgl {
	class ImageView : public View {
		private:
			static Shader* imageViewShader;
			Texture texture;
		public:
			ImageView(const std::string& textureFilename, float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
			ImageView(const Texture& texture, float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
			void setTexture(const Texture& texture);
			void setTexture(const std::string& textureFilename);
		protected:
			virtual void render(const Polygon& bounds, const glm::mat4& textureMapper);
	};
}

#endif
