#ifndef VIEW_H
#define VIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <list>
#include "rectangle.h"
#include "position.h"
#include "shader.h"
#include "color.h"
#include "camera.h"

class View {
	static Shader* viewShader;
	float verticies[8];
	unsigned int vao;
	Color backgroundColor;
	float rotation;
	glm::vec3 translation;
	glm::vec3 scalar;
	std::list<View*> subviews;
	bool clipSubviews;
	bool clipToParent;
	bool isScrollable;
	Position offsetPosition;
	public:
		View(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
		View(const View& view);
		virtual ~View();
		void setBounds(float x, float y, float width, float height);
		Rectangle getBounds() const;
		void setBackgroundColor(const Color& bc);
		void draw(float parentX, float parentY, float parentWidth, float ParentHeight);
		void addSubview(View* view);
		void setClipSubviews(bool clipSub);
		void setClipToParent(bool clipToP);
		bool getClipToParent() const;
		void setIsScrollable(bool s);
		bool getIsScrollable() const;
		void scroll(double xOffset, double yOffset);
		void translate(float x, float y);
		void setTranslation(float x, float y);
		void rotate(float radians);
		void setRotation(float radians);
		void scale(float x, float y);
		void setScalar(float x, float y);
		Position getOffsetPosition() const;
	protected:
		void setOffsetPosition(const Position& offset);
		virtual void onScroll(double xOffset, double yOffset);
		Shader* shader;
};

#endif
