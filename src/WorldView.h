#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <glm/glm.hpp>
#include <list>
#include "View.h"
#include "Cube.h"
#include "Polygon.h"
#include "Actor.h"

namespace cgl {
	class WorldView : public View {
		private:
			std::list<Actor*> actors;
		public:
			WorldView(float x, float y, float width, float height);
			void addActor(Actor* a);
			virtual void draw(const glm::mat4& parentModel, const Polygon& p);
	};
}

#endif