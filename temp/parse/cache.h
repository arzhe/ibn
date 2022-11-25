#ifndef IBN_PARSE_CACHE_H
#define IBN_PARSE_CACHE_H

#include <unordered_map>
#include <string>
#include <memory>

#include "mgr/domain_mgr.h"

namespace ibn {

class Cache {
public:
    Cache();

    static std::shared_ptr<Cache> NewCache();

    void SetDomain(const std::string& domain) {
        domain_ = domain;
    }
    
    void SetNodePair(const std::string& node1, const std::string& node2) {
        node_pair_ = std::make_pair(node1, node2);
    }

    const std::string& GetDomain() {
        return domain_;
    }

    std::pair<std::string, std::string> GetNodePair() {
        return node_pair_;
    }

private:
    std::string domain_;
    
    std::pair<std::string, std::string> node_pair_;
};

} // !namespace ibn

#endif // !IBN_PARSE_CACHE_H
