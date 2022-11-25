#ifndef IBN_CONFIG_CONFIG_H
#define IBN_CONFIG_CONFIG_H

#include <string>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

namespace ibn {

// Stores DOM tree parsed from the "path" file, which is generated by RapidJSON.
class Config {
public:
    Config();

    static const int BUFFERSIZE;

    static std::shared_ptr<Config> NewConfigFromFile(const std::string& path);

    void Parse();

    std::string GetPath() { return path_; }

    // DOM tree root
    Document dom_;

private:
    // json file's path
    std::string path_;
};

} // !namespace ibn

#endif // !IBN_CONFIG_CONFIG_H