#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "Model.h"
#include "Shader.h"

namespace cgl {
	class Actor {
		private:
			Model model;
		public:
			Actor(const std::string& path);
			virtual void draw(Shader& shader);
	};
}

#endif