#include <sstream>

#include "mgr/domain_mgr.h"
#include "base/util/util.h"
#include "log/logging.h"

namespace ibn {

Domain::Domain() {}

std::shared_ptr<Domain> Domain::NewDomain(int bandwidth, const std::vector<std::string>& topology) {
    auto dom = std::make_shared<Domain>();
    dom->bandwidth_ = bandwidth;
    dom->topology_ = topology;

    return dom;
}

bool Domain::IsLegalBandwidth(int bandwidth) {
    return !(bandwidth_ < bandwidth);
}

bool Domain::HasLink(const std::string& node1, const std::string& node2) {
    if(!node1.compare("") || !node2.compare("")) {
        return false;
    }
    for(const auto& s1 : topology_) {
        auto node1_pos = s1.find(node1);
        if(node1_pos == std::string::npos) {
            continue;
        }

        auto node2_pos = s1.find(node2, node1_pos);
        if(node2_pos == std::string::npos) {
            continue;
        }
        else {
            return true;
        }
    }
    return false;
}

DomainMgr::DomainMgr() {}

bool DomainMgr::HasDomain(const std::string& domain) {
    return dom_map_.find(domain) != dom_map_.end();
}

bool DomainMgr::IsLegalBandwidth(const std::string& domain, int bandwidth) {
    if(!HasDomain(domain)) {
        return false;
    }
    auto dom = dom_map_[domain];
    return dom->IsLegalBandwidth(bandwidth);
}

int DomainMgr::GetBandwidthByDomain(const std::string& domain) {
    if(!HasDomain(domain)) {
        return -1;
    }
    auto dom = dom_map_[domain];
    return dom->GetBandwidth();
}

bool DomainMgr::HasLink(const std::string& domain, const std::string& node1, const std::string& node2) {
    if(!HasDomain(domain)) {
        return false;
    }
    auto dom = dom_map_[domain];
    return dom->HasLink(node1, node2);
}

void DomainMgr::LoadDomainFromFile(const std::string& path) {
    auto cfg = Config::NewConfigFromFile(path);
    LoadDomainFromConfig(cfg);
}

void DomainMgr::LoadDomainFromConfig(std::shared_ptr<Config> cfg) {
    LoadData(this, cfg);   
}

std::shared_ptr<DomainMgr> DomainMgr::NewDomainMgr() {
    return std::make_shared<DomainMgr>();
}

std::shared_ptr<DomainMgr> DomainMgr::NewDomainMgrFromFile(const std::string& path) {
    auto dmgr = NewDomainMgr();
    dmgr->LoadDomainFromFile(path);
    return dmgr;
}

void DomainMgr::LoadData(DomainMgr* dmgr, std::shared_ptr<Config> cfg) {
    Value& domains = cfg->dom_["domain"];
    if(domains.IsArray()) {
        for(SizeType i = 0; i < domains.Size(); ++i) {
            Value& domain = domains[i];
            std::string dom_name; 
            int bandwidth;
            std::vector<std::string> topology;

            if(domain.HasMember("name") && domain["name"].IsString()) {
                dom_name = domain["name"].GetString();
            }
            else {
                LOG_SYSERR << "Wrong Json format : domain's name!";
            }

            if(domain.HasMember("bandwidth") && domain["bandwidth"].IsString()) {
                std::string bandwidth_str = IbnUtil::RTrim(domain["bandwidth"].GetString(), "Gb/s");
                std::stringstream ss;
                ss << bandwidth_str;
                ss >> bandwidth;
            }
            else {
                LOG_SYSERR << "Wrong Json format : domain's bandwidth!";
            }

            if(domain.HasMember("topology") && domain["topology"].IsArray()) {
                for(SizeType j = 0; j < domain["topology"].Size(); ++j) {
                    Value& path = domain["topology"][j];
                    if(path.HasMember("path") && path["path"].IsString()) {
                        topology.push_back(path["path"].GetString());
                    }
                    else {
                        LOG_SYSERR << "Wrong Json format : domain's path!";
                    }
                }
            }
            else {
                LOG_SYSERR << "Wrong Json format : domain's topology!";
            }

            auto dom = Domain::NewDomain(bandwidth, topology);
            dmgr->dom_map_[dom_name] = dom;
        }
    }
    else {
        LOG_SYSERR << "Wrong Json format : " << "should be \"domain\"";
    }
}

} // !namespace ibn
