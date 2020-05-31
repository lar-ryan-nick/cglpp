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
#include "SSAO.h"
#include "SSR.h"
#include "Pipeline.h"
#include "VAO.h"
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
		protected:
			// Fill G-Buffer
			virtual void geometryPass();
			// Fill shadow maps
			virtual void shadowPass();
			// Perform Screen Space operations
			virtual void screenSpacePass();
			// Perform lighting calculations
			virtual void lightingPass();
			// Perform post processing on shaded scene
			virtual void postProcessPass();
		private:
			static Shader* gBufferShader;
			static Shader* worldViewShader;
			static Shader* shadowMapShader;
			static Shader* reflectionShader;
			static Shader* canvasShader;
			GBuffer gBuffer;
			SSAO ssao;
			SSR ssr;
			std::list<Actor*> actors;
			Camera camera;
			float pitch;
			float yaw;
			CascadedShadowMap shadowMap;
			Pipeline lightingPipeline;
			Pipeline postProcessPipeline;
	};
}

#endif
