#include "../include/Shader.h"

cgl::Shader::Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath, const std::string& geometrySourcePath) {
	id = glCreateProgram();
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char* vertexShaderSource = getFileContents(vertexSourcePath);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	delete[] vertexShaderSource;
	vertexShaderSource = NULL;
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexSourcePath << '\n' << infoLog << std::endl;
	}
	glAttachShader(id, vertexShader);
	glDeleteShader(vertexShader);
	if (geometrySourcePath != "") {
		unsigned int geometryShader;
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		char* geometryShaderSource = getFileContents(geometrySourcePath);
		glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
		delete[] geometryShaderSource;
		geometryShaderSource = NULL;
		glCompileShader(geometryShader);
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << geometrySourcePath << '\n' << infoLog << std::endl;
		}
		glAttachShader(id, geometryShader);
		glDeleteShader(geometryShader);
	}
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char* fragmentShaderSource = getFileContents(fragmentSourcePath);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	delete[] fragmentShaderSource;
	fragmentShaderSource = NULL;
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentSourcePath << '\n' << infoLog << std::endl;
	}
	glAttachShader(id, fragmentShader);
	glDeleteShader(fragmentShader);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

cgl::Shader::Shader(unsigned int i) : id(i) {}

cgl::Shader::~Shader() {
	glDeleteProgram(id);
}

unsigned int cgl::Shader::getId() {
	return id;
}

void cgl::Shader::use() {
	glUseProgram(id);
}

void cgl::Shader::finish() {
	glUseProgram(0);
}

void cgl::Shader::setUniform(const std::string& name, float value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1f(location, value);
}

void cgl::Shader::setUniform(const std::string& name, float value1, float value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2f(location, value1, value2);
}

void cgl::Shader::setUniform(const std::string& name, float value1, float value2, float value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3f(location, value1, value2, value3);
}

void cgl::Shader::setUniform(const std::string& name, float value1, float value2, float value3, float value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4f(location, value1, value2, value3, value4);
}

void cgl::Shader::setUniform(const std::string& name, int value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, value);
}

void cgl::Shader::setUniform(const std::string& name, int value1, int value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2i(location, value1, value2);
}

void cgl::Shader::setUniform(const std::string& name, int value1, int value2, int value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3i(location, value1, value2, value3);
}

void cgl::Shader::setUniform(const std::string& name, int value1, int value2, int value3, int value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4i(location, value1, value2, value3, value4);
}

void cgl::Shader::setUniform(const std::string& name, unsigned int value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1ui(location, value);
}

void cgl::Shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2ui(location, value1, value2);
}

void cgl::Shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3ui(location, value1, value2, value3);
}

void cgl::Shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4ui(location, value1, value2, value3, value4);
}
/*
void cgl::Shader::setUniform(const std::string& name, double value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1d(location, value);
}

void cgl::Shader::setUniform(const std::string& name, double value1, double value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2d(location, value1, value2);
}

void cgl::Shader::setUniform(const std::string& name, double value1, double value2, double value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3d(location, value1, value2, value3);
}

void cgl::Shader::setUniform(const std::string& name, double value1, double value2, double value3, double value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4d(location, value1, value2, value3, value4);
}
*/

void cgl::Shader::setUniform(const std::string& name, const glm::vec2& value) {
	setUniform(name, value.x, value.y);
}

void cgl::Shader::setUniform(const std::string& name, const glm::vec3& value) {
	setUniform(name, value.x, value.y, value.z);
}

void cgl::Shader::setUniform(const std::string& name, const glm::vec4& value) {
	setUniform(name, value.x, value.y, value.z, value.w);
}

void cgl::Shader::setUniform(const std::string& name, const glm::mat4& m, bool transpose) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(m));
}

void cgl::Shader::setUniform(const std::string& name, const TextureMap& textureMap) {
	textureMap.getTexture().bind();
	setUniform(name + ".operation", textureMap.getOperation());
	setUniform(name + ".type", textureMap.getType());
	setUniform(name + ".strength", textureMap.getStrength());
	setUniform(name + ".uvIndex", textureMap.getUVIndex());
}

void cgl::Shader::setUniform(const std::string& name, const Material& material) {
	setUniform(name + ".ambientColor", static_cast<glm::vec3>(material.getAmbientColor()));
	setUniform(name + ".diffuseColor", static_cast<glm::vec3>(material.getDiffuseColor()));
	setUniform(name + ".specularColor", static_cast<glm::vec3>(material.getSpecularColor()));
	int i = 0;
	std::list<TextureMap> textureMaps = material.getTextureMaps();
	for (std::list<TextureMap>::iterator it = textureMaps.begin(); it != textureMaps.end(); it++) {
		std::stringstream ss;
		ss << name << ".textureMaps[" << i << "]";
		glActiveTexture(GL_TEXTURE0 + i);
		setUniform(ss.str(), *it);
		ss << ".texture";
		setUniform(ss.str(), i);
		++i;
	}
	while (i < 8) {
		std::stringstream ss;
		ss << name << ".textureMaps[" << i << "]";
		glActiveTexture(GL_TEXTURE0 + i);
		setUniform(ss.str(), TextureMap());
		ss << ".texture";
		setUniform(ss.str(), i);
		++i;
	}
	setUniform(name + ".shininess", material.getShininess());
	setUniform(name + ".opacity", material.getOpacity());
}

void cgl::Shader::setUniform(const std::string& name, const Light& light) {
	setUniform(name + ".color", static_cast<glm::vec3>(light.getColor()));
	setUniform(name + ".ambientStrength", light.getAmbientStrength());
	setUniform(name + ".diffuseStrength", light.getDiffuseStrength());
	setUniform(name + ".specularStrength", light.getSpecularStrength());
}

void cgl::Shader::setUniform(const std::string& name, const DirectionalLight& light) {
	setUniform(name, (Light) light);
	setUniform(name + ".direction", light.getDirection());
}

void cgl::Shader::setUniform(const std::string& name, const PointLight& light) {
	setUniform(name, (Light) light);
	setUniform(name + ".position", light.getPosition());
	setUniform(name + ".constant", light.getConstant());
	setUniform(name + ".linear", light.getLinear());
	setUniform(name + ".quadratic", light.getQuadratic());
}

void cgl::Shader::setUniform(const std::string& name, const SpotLight& light) {
	setUniform(name, (PointLight) light);
	setUniform(name + ".direction", light.getDirection());
	setUniform(name + ".cutOff", light.getCutOff());
	setUniform(name + ".outerCutOff", light.getOuterCutOff());
}

char* cgl::Shader::getFileContents(const std::string& filename) {
	std::ifstream fin(filename);
	if (fin.fail()) {
		std::cerr << "Couldn't open file " << filename << '\n';
	}
	fin.seekg(0, std::ios_base::end);
	int characterCount = fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	char* shaderSource = new char[characterCount];
	fin.get(shaderSource, characterCount, '\0');
	fin.close();
	return shaderSource;
}
