#include <stdio.h> // fopen fclose

#include "config/config.h"

namespace ibn {

const int Config::BUFFERSIZE = 65536;

Config::Config() {}

std::shared_ptr<Config> Config::NewConfigFromFile(const std::string& path) {
    auto cfg = std::make_shared<Config>();

    cfg->path_ = path;
    cfg->Parse();

    return cfg;
}

void Config::Parse() {
    FILE* fp = fopen(path_.data(), "r");
    char buffer[BUFFERSIZE];
    FileReadStream frs(fp, buffer, sizeof(buffer));

    dom_.ParseStream(frs);
    fclose(fp);
}

} // !ibn
