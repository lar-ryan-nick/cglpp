#ifndef CGL_SSAO_H
#define CGL_SSAO_H

#include "GBuffer.h"
#include "Shader.h"
#include <vector>
#include <random>

namespace cgl {
	class SSAO {
		public:
			SSAO();
			void draw(GBuffer& gBuffer, const glm::mat4& view, const glm::mat4& projection);
			void bindSSAO();
		private:
			static Shader* ssaoShader;
			static Shader* bilateralFilterShader;
			static unsigned int vao;
			static unsigned int vbo;
			static unsigned int ebo;
			static constexpr float SSIGMA = 10.0f;
			static constexpr float RSIGMA = 0.3f;
			static constexpr int BFK_SIZE = 7;
			static float normpdf(float x);
			static std::vector<float> bilateralFilterKernel;
			unsigned int noiseTexture;
			std::vector<glm::vec3> ssaoKernel;
			unsigned int ssaoFBO;
			unsigned int ssaoBuffer;
			unsigned int bilateralFilterFBO;
			unsigned int bilateralFilterBuffer;
	};
}

#endif
