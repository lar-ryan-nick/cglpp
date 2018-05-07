#ifndef VIEW_H
#define VIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "square.h"
#include "shader.h"
#include "color.h"

class View {
	Shader shader;
	Square square;
	Color backgroundColor;
	glm::mat4 model;
	public:
		View();
		void draw();
};

#endif
