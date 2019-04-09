#ifndef TEXTVIEW_H
#define	TEXTVIEW_H

#include <string>
#include "Font.h"
#include "View.h"
#include "Shader.h"
#include "Polygon.h"

namespace cgl {
	class TextView : public View {
		private:
			static Shader* textViewShader;
			unsigned int textVAO, textVBO;
			std::string text;
			Font font;
			static void init();
		public:
			TextView(const std::string& t = "", float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
			~TextView();
			void setText(const std::string& t);
			void setFont(const std::string& fontName);
			void setFont(const Font& f);
		protected:
			virtual void render(const Polygon& bounds, const glm::mat4& model);
	};
}

#endif
