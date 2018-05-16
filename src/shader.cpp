#include "shader.h"

Shader::Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath) {
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
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

unsigned int Shader::getId() {
	return id;
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::finish() {
	glUseProgram(0);
}

void Shader::setUniform(const std::string& name, float value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, float value1, float value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2f(location, value1, value2);
}

void Shader::setUniform(const std::string& name, float value1, float value2, float value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3f(location, value1, value2, value3);
}

void Shader::setUniform(const std::string& name, float value1, float value2, float value3, float value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4f(location, value1, value2, value3, value4);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) {
	setUniform(name, value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, int value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, int value1, int value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2i(location, value1, value2);
}

void Shader::setUniform(const std::string& name, int value1, int value2, int value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3i(location, value1, value2, value3);
}

void Shader::setUniform(const std::string& name, int value1, int value2, int value3, int value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4i(location, value1, value2, value3, value4);
}

void Shader::setUniform(const std::string& name, unsigned int value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1ui(location, value);
}

void Shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2ui(location, value1, value2);
}

void Shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3ui(location, value1, value2, value3);
}

void Shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4ui(location, value1, value2, value3, value4);
}

void Shader::setUniform(const std::string& name, double value) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform1d(location, value);
}

void Shader::setUniform(const std::string& name, double value1, double value2) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform2d(location, value1, value2);
}

void Shader::setUniform(const std::string& name, double value1, double value2, double value3) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform3d(location, value1, value2, value3);
}

void Shader::setUniform(const std::string& name, double value1, double value2, double value3, double value4) {
	int location = glGetUniformLocation(id, name.c_str());
	glUniform4d(location, value1, value2, value3, value4);
}

void Shader::setUniform(const std::string& name, unsigned int count, bool transpose, const float* value) {
	int location  = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, count, transpose, value);
}

void Shader::setUniform(const std::string& name, const Material& material) {
	setUniform(name + ".diffuseMap", 0);
	setUniform(name + ".specularMap", 1);
	setUniform(name + ".shininess", material.getShininess());
	material.bind();
}

void Shader::setUniform(const std::string& name, const Light& light) {
	setUniform(name + ".color", light.getColor());
	setUniform(name + ".ambientStrength", light.getAmbientStrength());
	setUniform(name + ".diffuseStrength", light.getDiffuseStrength());
	setUniform(name + ".specularStrength", light.getSpecularStrength());
}

void Shader::setUniform(const std::string& name, const DirectionalLight& light) {
	setUniform(name, (Light) light);
	setUniform(name + ".direction", light.getDirection());
}

void Shader::setUniform(const std::string& name, const PointLight& light) {
	setUniform(name, (Light) light);
	setUniform(name + ".position", light.getPosition());
	setUniform(name + ".constant", light.getConstant());
	setUniform(name + ".linear", light.getLinear());
	setUniform(name + ".quadratic", light.getQuadratic());
}

void Shader::setUniform(const std::string& name, const SpotLight& light) {
	setUniform(name, (PointLight) light);
	setUniform(name + ".direction", light.getDirection());
	setUniform(name + ".cutOff", light.getCutOff());
	setUniform(name + ".outerCutOff", light.getOuterCutOff());
}

char* Shader::getFileContents(std::string filename) {
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
