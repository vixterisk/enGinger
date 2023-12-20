#include "shader-program.hpp"
#include <iostream> 
#include <fstream>
#include <sstream>
#include <cstring>

/* create glsl shader needed type from given shader source code */
unsigned int createShader(GLenum shaderType, const char* shaderSource)
{
	unsigned int shader;
	/* creates an empty shader object and returns a non-zero value by which it can be referenced */
	shader = glCreateShader(shaderType);
	/* Replaces the source code in a shader object*/
	glShaderSource(shader, 1, &shaderSource, nullptr);
	/* Compiles a shader object with compilation status stored as part of the shader object's state (GL_COMPILE_STATUS) */
	glCompileShader(shader);
	int  success;
	char infoLog[512];
	/* reads parameter GL_COMPILE_STATUS value from shader into success */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		/* returns the shader object's information log modified when the shader is compiled */
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		const char* shaderTypeChar;
		if (shaderType == GL_VERTEX_SHADER)
			shaderTypeChar = "Vertex shader";
		else
			shaderTypeChar = "Fragment shader";
		// TODO remove console print
		std::cout << "::" << shaderTypeChar << ": compilation failed\n" << infoLog << std::endl;
		return 0;
	}
	return shader;
}

/* creates shader program with given vertex shader source code and fragment shader source code*/
GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	unsigned int shaderProgram;
	/* create shader program object and get its ID */
	shaderProgram = glCreateProgram();
	/* any number of shader objects can be attached at once as long as they have a different shader type. */
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	/* This step puts all shaders together and matches each output to each input. 
	The status of the link operation will be stored as part of the program object's state (GL_LINK_STATUS), and can fail for a number of reasons
	https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml */
	glLinkProgram(shaderProgram);

	int  success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
        char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        // TODO remove console print
		std::cout << "::Error: shader program compilation failed\n" << infoLog << std::endl;
	}
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

const char* readShaderFromFile(const std::string& shaderPath)
{
	std::ifstream file(shaderPath);
	if (!file)
	{
        // TODO remove console print
        char infoLog[512];
		std::cout << "::Error: shader file reading failed. " << strerror_s(infoLog, errno);
		return nullptr;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string stringBuffer = buffer.str();
    int bufferLength = stringBuffer.length();
	char* result = new char[bufferLength];
	memcpy(result, stringBuffer.c_str(), bufferLength * sizeof(char) + 1);
	return result;
}

/* Reads vertex shader located in 'resources/shaders/vertexShaderName'
and fragment shader located in 'resourcess/shaders/fragmentShaderName', then creates shader program from these shaders */
GLuint createShaderProgramUsingFile(const std::string& vertexShaderAbsolutePath, const std::string& fragmentShaderAbsolutePath)
{
	const char* vertexShaderSource = readShaderFromFile(vertexShaderAbsolutePath);
	const char* fragmentShaderSource = readShaderFromFile(fragmentShaderAbsolutePath);
	if (vertexShaderSource == nullptr || fragmentShaderSource == nullptr)
		return 0;
	GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	return shaderProgram;
}