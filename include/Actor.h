#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Polygon.h"

namespace cgl {
	class Actor {
		private:
			Model model;
		public:
			Actor(const std::string& path);
			virtual ~Actor();
			virtual void draw(Shader& shader, const glm::mat4& parentModel);
			Model& getModel();
	};
}

#endif
