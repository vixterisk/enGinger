#include <string>

struct ConfigData
{
	std::string vertexShader;
	std::string fragmentShader;
	bool fullscreen;
	bool borderless;
	int width;
	int height;
};

ConfigData readConfig();