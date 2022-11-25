#include "parse/cache.h"

namespace ibn {

std::shared_ptr<Cache> Cache::NewCache() {
    return std::make_shared<Cache>();
}

void Cache::SetNodePair(const std::string& node1, const std::string& node2) {
    node_pair_ = std::make_pair(node1, node2);
}

} // !namespace ibn
