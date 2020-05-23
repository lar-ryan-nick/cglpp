#ifndef CGL_WORLDVIEW_H
#define CGL_WORLDVIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <sstream>
#include "View.h"
#include "Cube.h"
#include "Polygon.h"
#include "Actor.h"
#include "Shader.h"
#include "Camera.h"
#include "CascadedShadowMap.h"
#include "GBuffer.h"
#include <limits>

namespace cgl {
	class WorldView : public View {
		public:
			WorldView(float x, float y, float width, float height);
			void addActor(Actor* a);
			virtual void draw(const glm::mat4& parentModel, const Polygon& poly);
			virtual void render(const Polygon& bounds, const glm::mat4& model);
			virtual void onMouseMove(float xOffset, float yOffset);
			Camera getCamera();
			void setCamera(const Camera& c);
		private:
			static Shader* gBufferShader;
			static Shader* worldViewShader;
			static Shader* shadowMapShader;
			GBuffer gBuffer;
			std::list<Actor*> actors;
			Camera camera;
			float pitch;
			float yaw;
			CascadedShadowMap shadowMap;
	};
}

#endif
