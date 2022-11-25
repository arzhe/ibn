#ifndef IBN_MGR_POLICY_MGR_H
#define IBN_MGR_POLICY_MGR_H

#include "mgr/domain_mgr.h"
#include "mgr/node_mgr.h"

namespace ibn {

class PolicyMgr {
public:
    PolicyMgr();

    PolicyMgr(const std::string& path);

    static std::shared_ptr<PolicyMgr> NewPolicyMgr();

    static std::shared_ptr<PolicyMgr> NewPolicyMgrFromFile(const std::string& path);

    bool HasDomain(const std::string& domain);
    std::vector<std::string> GetAllDomains();
    bool HasDomainBySimilarity(const std::string& domain, int sim = SIM);
    std::vector<std::string> GetAllDomainsBySimilarity(const std::string& domain, int sim = SIM);

    bool HasNodeByName(const std::string& name);
    std::vector<std::string> GetAllNodesName();
    bool HasNodeByNameAndSimilarity(const std::string& name, int sim = SIM);
    std::vector<std::string> GetAllNodesByNameAndSimilarity(const std::string& name, int sim = SIM);

    bool HasNodeById(const std::string& id);

    // node1: Wuhan , node2: Guandong.
    bool HasLink(const std::string& domain, const std::string& node1, const std::string& node2);

    bool IsLegalBandwidth(const std::string& domain, int bandwidth);
    int GetBandwidthByDomain(const std::string& domain);
    
    static const int SIM;
private:
    std::shared_ptr<DomainMgr> dmgr_;
    std::shared_ptr<NodeMgr> nmgr_;

};

} // !namespace ibn


#endif // !IBN_MGR_POLICY_MGR_H
