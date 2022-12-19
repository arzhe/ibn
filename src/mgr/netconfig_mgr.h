#ifndef IBN_MGR_NETCONFIG_MGR_H
#define IBN_MGR_NETCONFIG_MGR_H

#include <string>
#include <unordered_map>
#include <memory>
#include <tuple>

namespace ibn {

enum Meta {
    DOMAIN,
    TOPO,
    BANDWIDTH
};


class NetData {
public:
    NetData(const std::string& domain, const std::string& topo, int bandwidth);
    ~NetData();

    template<Meta index>
    auto Get() {
        return std::get<index>(data_);
    }

private:
    std::tuple<std::string, std::string, int> data_;

};

class NetConfigMgr {
public:
    NetConfigMgr()
    ~NetConfigMgr()

    

private:
    std::unordered_map<NetData, int> netdata_delay_map_;
};

} // !namespace ibn

#endif // !IBN_MGR_NETCONFIG_MGR_H
