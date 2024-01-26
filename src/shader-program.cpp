#include "shader-program.hpp"

#include <iostream> 
#include <fstream>
#include <sstream>
#include <cstring>
#include <map>

std::map<GLuint , std::string> shaderTypeName
{
    { GL_VERTEX_SHADER, "Vertex shader" },
    { GL_FRAGMENT_SHADER, "Fragment shader" }
};

unsigned int createShader(GLenum shaderType, const char* shaderSource)                                                   // Creates glsl shader needed type from given shader source code.
{
	unsigned int shader = glCreateShader(shaderType);                                                               // Creates an empty shader object and returns a non-zero value by which it can be referenced.
	glShaderSource(shader, 1, &shaderSource, nullptr);                                                 // Replaces the source code in a shader object.
    glCompileShader(shader);                                                                                             // Compiles a shader object with compilation status stored as part of the shader object's state (GL_COMPILE_STATUS).

    int  success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);                                                    // Reads parameter GL_COMPILE_STATUS value from shader into success.
	if (!success) {
        char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);                                                 // Returns the shader object's information log modified when the shader is compiled.
		std::cout << "::" << shaderTypeName[shaderType] << ": compilation failed\n" << infoLog << std::endl;
		return 0;
	}

	return shader;
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)                             // Creates shader program with given vertex shader source code and fragment shader source code.
{
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	unsigned int shaderProgram = glCreateProgram();                                                                      // Creates shader program object and get its ID.

	glAttachShader(shaderProgram, vertexShader);                                                          // Any number of shader objects can be attached at once as long as they have a different shader type.
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);                                                                                // This step puts all shaders together and matches each output to each input.
                                                                                                                         // The status of the link operation will be stored as part of the program object's state (GL_LINK_STATUS), and can fail for a number of reasons.
    int  success;                                                                                                        // more information can be obtained at https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
        char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "::Error: shader program compilation failed\n" << infoLog << std::endl;
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

const char* readShaderFromFile(const std::string &shaderPath)
{
	std::ifstream file(shaderPath);
	if (!file) {
        char infoLog[512];
		std::cout << "::Error: shader file reading failed. " << strerror_s(infoLog, errno);               // TODO change all std::cout to log system?
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

GLuint createShaderProgramUsingFile(                                                                                     // Reads vertex shader located in 'resources/shaders/vertexShaderName'
        const std::string &vertexShaderAbsolutePath,                                                                     // and fragment shader located in 'resourcess/shaders/fragmentShaderName', then creates shader program from these shaders.
        const std::string &fragmentShaderAbsolutePath
    )
{
	const char* vertexShaderSource = readShaderFromFile(vertexShaderAbsolutePath);
	const char* fragmentShaderSource = readShaderFromFile(fragmentShaderAbsolutePath);

	if (vertexShaderSource == nullptr || fragmentShaderSource == nullptr)
		return 0;

	GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	return shaderProgram;
}