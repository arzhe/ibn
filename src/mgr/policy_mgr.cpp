#include "mgr/policy_mgr.h"
#include "base/util/util.h"
#include "log/logging.h"

namespace ibn {

const int PolicyMgr::SIM = 90;

PolicyMgr::PolicyMgr() {}

PolicyMgr::PolicyMgr(const std::string& path) {
    dmgr_ = DomainMgr::NewDomainMgrFromFile(path);
    nmgr_ = NodeMgr::NewNodeMgrFromFile(path);
}

std::shared_ptr<PolicyMgr> PolicyMgr::NewPolicyMgr() {
    return std::make_shared<PolicyMgr>();
}

std::shared_ptr<PolicyMgr> PolicyMgr::NewPolicyMgrFromFile(const std::string& path) {
    return std::make_shared<PolicyMgr>(path);
}

bool PolicyMgr::HasDomain(const std::string& domain) {
    return dmgr_->HasDomain(domain);
}

std::vector<std::string> PolicyMgr::GetAllDomains() {
    std::vector<std::string> dom_arr;

    for(const auto& domain : dmgr_->dom_map_) {
        dom_arr.push_back(domain.first);
    }
    
    return dom_arr;
}

bool PolicyMgr::HasDomainBySimilarity(const std::string& domain, int sim) {
    for(const auto& d : dmgr_->dom_map_) {
        if(IbnUtil::Similarity(domain, d.first) >= sim) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> PolicyMgr::GetAllDomainsBySimilarity(const std::string& domain, int sim) {
    std::vector<std::string> res;
    for(const auto& d : dmgr_->dom_map_) {
        if(IbnUtil::Similarity(domain, d.first) >= sim) {
            res.emplace_back(d.first);
        }
    }

    std::sort(res.begin(), res.end(),
              [domain](const std::string& a, const std::string& b) {
                return IbnUtil::Similarity(domain, a) > IbnUtil::Similarity(domain, b);
              });

    return res;
}

bool PolicyMgr::HasNodeByName(const std::string& name) {
    return nmgr_->HasNodeByName(name);
}

std::vector<std::string> PolicyMgr::GetAllNodesName() {
    std::vector<std::string> res;
    for(const auto& n : nmgr_->node_name_id_map_) {
        res.emplace_back(n.first);
    }
    return res;
}

bool PolicyMgr::HasNodeByNameAndSimilarity(const std::string& name, int sim) {
    for(const auto& n : nmgr_->node_name_id_map_) {
        auto rat = IbnUtil::Similarity(name, n.first);
        if(rat >= sim) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> PolicyMgr::GetAllNodesByNameAndSimilarity(const std::string& name, int sim) {
    std::vector<std::string> res;
    for(const auto& n : nmgr_->node_name_id_map_) {
        if(IbnUtil::Similarity(name, n.first) >= sim) {
            res.emplace_back(n.first);
        }
    }

    std::sort(res.begin(), res.end(),
              [name](const std::string& a, const std::string& b) {
                return IbnUtil::Similarity(name, a) > IbnUtil::Similarity(name, b);
              });

    return res;
}

bool PolicyMgr::HasNodeById(const std::string& id) {
    return nmgr_->HasNodeById(id);
}

bool PolicyMgr::HasLink(const std::string& domain, const std::string& node1, const std::string& node2) {
    std::string name1 = nmgr_->GetNodeIdByName(node1);
    std::string name2 = nmgr_->GetNodeIdByName(node2);
    return dmgr_->HasLink(domain, name1, name2);
}

bool PolicyMgr::IsLegalBandwidth(const std::string& domain, int bandwidth) {
    return dmgr_->IsLegalBandwidth(domain, bandwidth);
}

int PolicyMgr::GetBandwidthByDomain(const std::string& domain) {
    return dmgr_->GetBandwidthByDomain(domain);
}

} // !namespace ibn
