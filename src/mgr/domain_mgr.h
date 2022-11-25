#ifndef IBN_MGR_DOMAIN_MGR_H
#define IBN_MGR_DOMAIN_MGR_H

#include <unordered_map>
#include <vector>

#include "config/config.h"

namespace ibn {

// Describes a single domain, eg: "demo".
class Domain {
public:
    Domain();

    static std::shared_ptr<Domain> NewDomain(int bandwidth, const std::vector<std::string>& topology);
    
    bool IsLegalBandwidth(int bandwidth);
    int GetBandwidth() { return bandwidth_; }

    // eg:node1 : "1", node2 : "2", but not "Wuhan" or "Guandong".
    bool HasLink(const std::string& node1, const std::string& node2);

private:
    int bandwidth_;

    std::vector<std::string> topology_;

};

// Management for domain, eg: "domo" and "test" in json file.
class DomainMgr {
public:
    DomainMgr();

    bool HasDomain(const std::string& domain);

    bool IsLegalBandwidth(const std::string& domain, int bandwidth);
    int GetBandwidthByDomain(const std::string& domain);

    bool HasLink(const std::string& domain, const std::string& node1, const std::string& node2);

    static std::shared_ptr<DomainMgr> NewDomainMgr();

    static std::shared_ptr<DomainMgr> NewDomainMgrFromFile(const std::string& path);

    std::unordered_map<std::string, std::shared_ptr<Domain>> dom_map_;

private:
    void LoadDomainFromFile(const std::string& path);

    void LoadDomainFromConfig(std::shared_ptr<Config> cfg);

    static void LoadData(DomainMgr* dmgr, std::shared_ptr<Config> cfg);

};

} // !namespace ibn

#endif // !IBN_MGR_DOMAIN_MGR_H
