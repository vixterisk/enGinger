#include "path.hpp"
#include "config-data.hpp"

void readValue(nlohmann::json data, std::string jsonKey, int &result)
{
    nlohmann::json value = data[jsonKey];
    result = value.template get<int>();
}

void readValue(nlohmann::json data, std::string jsonKey, std::string &result)
{
    nlohmann::json value = data[jsonKey];
    result = value.template get<std::string>();
}

void readValue(nlohmann::json data, std::string jsonKey, bool &result)
{
    nlohmann::json value = data[jsonKey];
    result = value.template get<bool>();
}

ConfigData readConfig()
{
    using json = nlohmann::json;
    std::string configJson = getAbsolutePath(PathNodeType::configJson);
    std::ifstream config(configJson);
    json data = json::parse(config);

    ConfigData result;

    readValue(data, "vertexShader", result.vertexShader);
    readValue(data, "fragmentShader", result.fragmentShader);
    readValue(data, "fullscreen", result.fullscreen);
    readValue(data, "borderless", result.borderless);
    readValue(data, "width", result.width);
    readValue(data, "height", result.height);

    return result;
}
