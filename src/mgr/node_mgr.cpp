#include <sstream>

#include "mgr/node_mgr.h"
#include "base/util/util.h"
#include "log/logging.h"

namespace ibn {

NodeMgr::NodeMgr() {}

bool NodeMgr::HasNodeById(const std::string& id) {
    for(const auto& name_id : node_name_id_map_) {
        if(name_id.second == id) {
            return true;
        }
    }
    return false;
}

bool NodeMgr::HasNodeByName(const std::string& name) {
    return node_name_id_map_.find(name) != node_name_id_map_.end();
}

std::string NodeMgr::GetNodeNameById(const std::string& id) {
    if(!HasNodeById(id)) {
        LOG_TRACE << "Cannot find node's name by id: " << "[" << id << "]";
        return "";
    }

    for(const auto& name_id : node_name_id_map_) {
        if(name_id.second == id) {
            return name_id.first;
        }
    }
}

std::string NodeMgr::GetNodeIdByName(const std::string& name) {
    if(!HasNodeByName(name)) {
        LOG_TRACE << "Cannot find node's id by name: " << "[" << name << "]";
        return "";
    }

    return node_name_id_map_[name];
}

std::shared_ptr<NodeMgr> NodeMgr::NewNodeMgr() {
    return std::make_shared<NodeMgr>();
}

std::shared_ptr<NodeMgr> NodeMgr::NewNodeMgrFromFile(const std::string& path) {
    auto nmgr = NodeMgr::NewNodeMgr();
    nmgr->LoadNodeFromFile(path);
    return nmgr;
}

void NodeMgr::LoadNodeFromFile(const std::string& path) {
    auto cfg = Config::NewConfigFromFile(path);
    LoadNodeFromConfig(cfg);
}

void NodeMgr::LoadNodeFromConfig(std::shared_ptr<Config> cfg) {
    LoadData(this, cfg);
}

void NodeMgr::LoadData(NodeMgr* nmgr, std::shared_ptr<Config> cfg) {
    Value& nodes = cfg->dom_["node"];

    if(nodes.IsArray()) {
        for(SizeType i = 0; i < nodes.Size(); ++i) {
            Value& node = nodes[i];

            std::string name;
            std::string id;
            std::string ip;

            if(node.HasMember("name") && node["name"].IsString()) {
                name = node["name"].GetString();
            }
            else {
                LOG_SYSERR << "Wrong Json format : node's name!";
            }

            if(node.HasMember("id") && node["id"].IsInt()) {
                std::stringstream ss;
                ss << node["id"].GetInt();
                ss >> id;
            }
            else {
                LOG_SYSERR << "Wrong Json format : node's id!";
            }

            if(node.HasMember("ip") && node["ip"].IsString()) {
                ip = node["ip"].GetString();
            }
            else {
                LOG_SYSERR << "Wrong Json format : node's ip!";
            }

            nmgr->node_name_id_map_[name] = id;
            nmgr->node_name_ip_map_[name] = ip;
        }
    }
    else {
        LOG_SYSERR << "Wrong Json format : " << "should be \"node\"";
    }
}

} // !namespace ibn
