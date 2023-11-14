#include "ShaderProgram.h"

/* create glsl shader needed type from given shader source code */
unsigned int CreateShader(GLenum shaderType, const char* shaderSource)
{
	unsigned int shader;
	/* creates an empty shader object and returns a non-zero value by which it can be referenced */
	shader = glCreateShader(shaderType);
	/* Replaces the source code in a shader object*/
	glShaderSource(shader, 1, &shaderSource, NULL);
	/* Compiles a shader object with compilation status stored as part of the shader object's state (GL_COMPILE_STATUS) */
	glCompileShader(shader);
	int  success;
	char infoLog[512];
	/* reads parameter GL_COMPILE_STATUS value from shader into success */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		/* returns the shader object's information log modified when the shader is compiled */
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

/* creates shader program with given vertex shader source code and fragment shader source code*/
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

/* reads shader source code from file, located in '$resourcesPath/shaders/shaderName' */
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

/* Reads vertex shader located in 'resourcesPath/shaders/vertexShaderName' 
and fragment shader located in 'resourcesPath/shaders/fragmentShaderName', then creates shader program from these shaders */
GLuint CreateShaderProgram(std::string resourcesPath, std::string vertexShaderName, std::string fragmentShaderName)
{
	const char* vertexShaderSource = readShaderFromFile(resourcesPath, vertexShaderName);
	const char* fragmentShaderSource = readShaderFromFile(resourcesPath, fragmentShaderName);
	GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
//	delete vertexShaderSource;
//	delete fragmentShaderSource;
	return shaderProgram;
}