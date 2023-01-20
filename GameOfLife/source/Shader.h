#pragma once
#include "main.h"

#include <fstream>
#include <string>

class Shader {
public:
	// Id to identify individual shader
	unsigned int ShaderID;

	Shader(const char* vertexShPath, const char* fragmentShPath);

	void use();

	void setFloat(const std::string &uniformName, float floatValue);
	void setInt(const std::string &uniformName, int intValue);
	void setBool(const std::string &uniformName, bool boolValue);
	void setMat4(const std::string &uniformName, glm::mat4 &matValue);
	void setVec3(const std::string &uniformName, glm::vec3 &vecValue);
};
