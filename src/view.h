#ifndef VIEW_H
#define VIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rectangle.h"
#include "shader.h"
#include "color.h"

class View {
	Shader shader;
	float verticies[8];
	unsigned int vao;
	Color backgroundColor;
	glm::mat4 model;
	public:
		View(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f);
		void setBounds(float x, float y, float width, float height);
		Rectangle getBounds() const;
		void setBackgroundColor(const Color& bc);
		void draw(float parentX, float parentY, float parentWidth, float ParentHeight);
};

#endif
