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

//
//Shader::Shader(const char* vertexShPath, const char* fragmentShPath) {
//	std::string vertexShaderSource;
//	std::string fragmentShaderSource;
//	std::string inputLine;
//	int success;
//	char errorLog[512];
//
//	std::ifstream vertexShaderSourceStream(vertexShPath);
//	if (!vertexShaderSourceStream.is_open()) {
//		std::cout << "ERROR :: COULD NOT OPEN VERTEX SHADER SOURCE FILE \n";
//		vertexShaderSource = "";
//	}
//	else {
//		while (getline(vertexShaderSourceStream, inputLine)) {
//			vertexShaderSource += inputLine + "\n";
//		}
//
//	}
//
//	std::ifstream fragmentShaderSourceStream(fragmentShPath);
//	if (!fragmentShaderSourceStream.is_open()) {
//		std::cout << "ERROR :: COULD NOT OPEN FRAGMENT SHADER SOURCE FILE \n";
//		fragmentShaderSource = "";
//	}
//	else {
//		while (getline(fragmentShaderSourceStream, inputLine)) {
//			fragmentShaderSource += inputLine + "\n";
//		}
//	}
//	//convert strings to const char* to be passed in to the glshadersource function
//	const char* fShaderChar = fragmentShaderSource.c_str();
//	const char* vShaderChar = vertexShaderSource.c_str();
//
//	vertexShaderSourceStream.close();
//	fragmentShaderSourceStream.close();
//
//	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vShaderChar, NULL);
//	glCompileShader(vertexShader);
//
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cout << "ERROR :: VERTEX SHADER COMPILATION FAILED\n" << errorLog << std::endl;
//		return;
//	}
//
//	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fShaderChar, NULL);
//	glCompileShader(fragmentShader);
//
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cout << "ERROR :: FRAGEMENT SHADER COMPILATION FAILED\n" << errorLog << std::endl;
//		return;
//	}
//
//	ShaderID = glCreateProgram();
//	glAttachShader(ShaderID, vertexShader);
//	glAttachShader(ShaderID, fragmentShader);
//	glLinkProgram(ShaderID);
//
//	glGetShaderiv(ShaderID, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(ShaderID, 512, NULL, errorLog);
//		std::cout << "ERROR :: LINKING SHADER FAILED\n" << errorLog << std::endl;
//	}
//
//	glDeleteShader(fragmentShader);
//	glDeleteShader(vertexShader);
//}
//
//void Shader::use() {
//	glUseProgram(ShaderID);
//}
//
//void Shader::setFloat(const std::string &uniformName, float floatValue) {
//	glUniform1f(glGetUniformLocation(ShaderID, uniformName.c_str()), floatValue);
//}
//
//void Shader::setInt(const std::string &uniformName, int intValue) {
//	glUniform1i(glGetUniformLocation(ShaderID, uniformName.c_str()), intValue);
//}
//
//void Shader::setBool(const std::string &uniformName, bool boolValue) {
//	//cast to an int since there is no gluniformli for
//	glUniform1i(glGetUniformLocation(ShaderID, uniformName.c_str()), (int)boolValue);
//}
//
//void Shader::setMat4(const std::string &uniformName, glm::mat4 &matValue) {
//	glUniformMatrix4fv(glGetUniformLocation(ShaderID, uniformName.c_str()), 1, GL_FALSE, &matValue[0][0]);
//}
//
//void Shader::setVec3(const std::string &uniformName, glm::vec3 &vecValue) {
//	glUniform3f(glGetUniformLocation(ShaderID, uniformName.c_str()), vecValue.x, vecValue.y, vecValue.z);
//}