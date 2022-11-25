#ifndef IBN_MGR_NODE_MGR_H
#define IBN_MGR_NODE_MGR_H

#include <unordered_map>
#include <string>

#include "config/config.h"

namespace ibn {

class NodeMgr {
public:
    NodeMgr();
    
    bool HasNodeById(const std::string& id);

    bool HasNodeByName(const std::string& name);

    std::string GetNodeNameById(const std::string& id);

    std::string GetNodeIdByName(const std::string& name);

    static std::shared_ptr<NodeMgr> NewNodeMgr();

    static std::shared_ptr<NodeMgr> NewNodeMgrFromFile(const std::string& path);

    // eg: id:1, name:Wuhan.
    std::unordered_map<std::string, std::string> node_name_id_map_;

    // eg: 172.16.100.1/24
    std::unordered_map<std::string, std::string> node_name_ip_map_;

private:
    void LoadNodeFromFile(const std::string& path);

    void LoadNodeFromConfig(std::shared_ptr<Config> cfg);

    static void LoadData(NodeMgr* nmgr, std::shared_ptr<Config> cfg);

};
  


} // !namespace ibn

#endif // !IBN_MGR_NODE_MGR_H
