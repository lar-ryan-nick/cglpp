#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <string>
#include "view.h"
#include "texture.h"

class ImageView : public View {
	static Shader* imageViewShader;
	Texture texture;
	public:
		ImageView(const std::string& textureFilename, float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
		ImageView(const Texture& texture, float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
		void setTexture(const Texture& texture);
		void setTexture(const std::string& textureFilename);
		virtual void draw(float parentX, float parentY, float parentWidth, float parentHeight, const glm::mat4& parentModel);
};

#endif
