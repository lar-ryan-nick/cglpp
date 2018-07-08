#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include "Polygon.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace cgl {
	class Actor {
		private:
		public:
			Actor();
			virtual void draw(const glm::mat4& parentModel, const Polygon& p);
	};
}

#endif