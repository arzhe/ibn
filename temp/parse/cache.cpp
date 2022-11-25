#include "parse/cache.h"

namespace ibn {

Cache::Cache() {}

std::shared_ptr<Cache> Cache::NewCache() {
    return std::make_shared<Cache>();
}

} // !namespace ibn
