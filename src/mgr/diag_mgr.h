#ifndef IBN_PARSE_DIAG_MGR_H
#define IBN_PARSE_DIAG_MGR_H

#include <memory>
#include <unordered_map>

#include "config/config.h"

namespace ibn {

struct EnumClassHash {
    template<typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

enum class Diag {
    domain,
    subdomain,
    service,
    subservice,
    servicerollback,
    bandwidth,
    bandwidthrollback
};

template<typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template<typename Key, typename T>
using UnorderedMap = std::unordered_map<Key, T, HashType<Key>>;

class DiagMgr {
public:
    static std::shared_ptr<DiagMgr> NewDiagMgr();
    
    void Init();

    void Parse();

    std::string GetDiag(Diag diag, const std::string& condition);

    static const std::string path_;

private:
    bool HasDiag(Diag diag, const std::string& condition);

    std::shared_ptr<Config> cfg_;
    
    UnorderedMap<Diag, std::unordered_map<std::string, std::string>> diag_map_;
    /* std::unordered_map<Diag, std::unordered_map<std::string, std::string>> diag_map_; */
};

} // !namespace ibn

#endif // !IBN_PARSE_DIAG_MGR_H
