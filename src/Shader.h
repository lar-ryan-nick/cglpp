#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace cgl {
	class Shader {
		private:
			unsigned int id;
			char* getFileContents(const std::string& filename);
		public:
			Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath, const std::string& geometrySourcePath = "");
			Shader(unsigned int i);
			~Shader();
			unsigned int getId();
			void use();
			void finish();
			void setUniform(const std::string& name, float value);
			void setUniform(const std::string& name, float value1, float value2);
			void setUniform(const std::string& name, float value1, float value2, float value3);
			void setUniform(const std::string& name, float value1, float value2, float value3, float value4);
			void setUniform(const std::string& name, int value);
			void setUniform(const std::string& name, int value1, int value2);
			void setUniform(const std::string& name, int value1, int value2, int value3);
			void setUniform(const std::string& name, int value1, int value2, int value3, int value4);
			void setUniform(const std::string& name, unsigned int value);
			void setUniform(const std::string& name, unsigned int value1, unsigned int value2);
			void setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3);
			void setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4);
			/*
			void setUniform(const std::string& name, double value);
			void setUniform(const std::string& name, double value1, double value2);
			void setUniform(const std::string& name, double value1, double value2, double value3);
			void setUniform(const std::string& name, double value1, double value2, double value3, double value4);
			*/
			void setUniform(const std::string& name, const glm::vec2& value);
			void setUniform(const std::string& name, const glm::vec3& value);
			void setUniform(const std::string& name, const glm::vec4& value);
			void setUniform(const std::string& name, const glm::mat4& m, bool transpose = false);
			void setUniform(const std::string& name, const Material& material);
			void setUniform(const std::string& name, const Light& light);
			void setUniform(const std::string& name, const DirectionalLight& light);
			void setUniform(const std::string& name, const PointLight& light);
			void setUniform(const std::string& name, const SpotLight& light);
	};
}

#endif
