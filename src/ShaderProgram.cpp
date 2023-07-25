#include "ShaderProgram.h"

unsigned int CreateShader(GLenum shaderType, const char* shaderSource)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		const char* shaderTypeChar;
		if (shaderType == GL_VERTEX_SHADER)
			shaderTypeChar = "Vertex shader";
		else
			shaderTypeChar = "Fragment shader";
		std::cout << "::" << shaderTypeChar << ": compilation failed\n" << infoLog << std::endl;
		return NULL;
	}
	return shader;
}

const char* readShaderFromFile(std::string resourcesPath, std::string shaderName)
{
	std::string fullFilePath = resourcesPath.append("\\shaders\\").append(shaderName);
	std::ifstream file(fullFilePath);
	if (!file) {
		std::cout << "::Error: " << strerror(errno);
		return 0;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string stringBuffer = buffer.str();
	int bufferLength = stringBuffer.length();
	char* result = new char[bufferLength] + 1;
	memcpy(result, stringBuffer.c_str(), bufferLength * sizeof(char) + 1);
	return result;
}

// Public

GLuint CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "::Error: shader program compilation failed\n" << infoLog << std::endl;
	}
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLuint CreateShaderProgram(std::string resourcesPath, std::string vertexShaderName, std::string fragmentShaderName)
{
	const char* vertexShaderSource = readShaderFromFile(resourcesPath, vertexShaderName);
	const char* fragmentShaderSource = readShaderFromFile(resourcesPath, fragmentShaderName);
	return CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}