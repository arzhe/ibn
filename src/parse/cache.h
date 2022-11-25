#ifndef IBN_PARSE_CACHE_H
#define IBN_PARSE_CACHE_H

#include <string>
#include <memory>
#include <utility>

namespace ibn {

class Cache {
public:
    static std::shared_ptr<Cache> NewCache();
    
    void SetDomain(const std::string& domain) { domain_ = domain; }
    std::string GetDomain() { return domain_; }

    void SetNodePair(const std::string& node1, const std::string& node2);
    const std::string& GetFirstNode() { return node_pair_.first; }
    const std::string& GetSecondNode() { return node_pair_.second; }

    void SetBandwidth(int bandwidth) { bandwidth_ = bandwidth; }
    int GetBandwidth() { return bandwidth_; }

private:
    std::string domain_;

    std::pair<std::string, std::string> node_pair_;

    int bandwidth_;
    
};

} // !namespace ibn

#endif // !IBN_PARSE_CACHE_H
