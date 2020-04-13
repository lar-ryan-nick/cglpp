#ifndef VIEW_H
#define VIEW_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <list>
#include "Rectangle.h"
#include "Position.h"
#include "Size.h"
#include "Shader.h"
#include "Color.h"
#include "Polygon.h"
#include "Transformable.h"

namespace cgl {
	class View : public Transformable {
		private:
			static Shader* viewShader;
			const static unsigned int MAX_VERTICIES;
			Rectangle bounds;
			Color backgroundColor;
			std::list<View*> subviews;
			bool clipSubviews;
			bool clipToParent;
			bool isScrollable;
			bool isClickable;
			bool isPressed;
			Position offsetPosition;
		public:
			View(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
			View(const View& view);
			virtual ~View();
			void setBounds(float x, float y, float width, float height);
			Rectangle getBounds() const;
			void setBackgroundColor(const Color& bc);
			virtual void draw(const glm::mat4& parentModel, const Polygon& poly);
			void addSubview(View* view);
			void setClipSubviews(bool clipSub);
			void setClipToParent(bool clipToP);
			bool getClipToParent() const;
			void setIsScrollable(bool s);
			bool getIsScrollable() const;
			bool scroll(double xOffset, double yOffset, float mouseX, float mouseY);
			bool mousePress(int button, int mods, float mouseX, float mouseY);
			bool mouseRelease(int button, int mods, float mouseX, float mouseY);
			void propogateMouseRelease();
			Position getOffsetPosition() const;
			virtual void onMouseMove(float xOffset, float yOffset);
		protected:
			unsigned int vao;
			unsigned int vbo;
			unsigned int ebo;
			void setOffsetPosition(const Position& offset);
			virtual bool onScroll(double xOffset, double yOffset);
			virtual bool onMousePress(int button, int mods);
			virtual bool onMouseRelease(int button, int mods);
			virtual void onClick();
			virtual void render(const cgl::Polygon& bounds, const glm::mat4& model);
	};
}

#endif
