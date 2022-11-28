#include "mgr/diag_mgr.h"
#include "log/logging.h"

namespace ibn {

const std::string DiagMgr::path_ = "../../doc/intend.json";

std::shared_ptr<DiagMgr> DiagMgr::NewDiagMgr() {
    auto dmg = std::make_shared<DiagMgr>();
    dmg->Init();
    dmg->Parse();

    return dmg;
}

void DiagMgr::Init() {
    cfg_ = Config::NewConfigFromFile(DiagMgr::path_);
}

void DiagMgr::Parse() {
    Document& json = cfg_->dom_;
    if(json.HasMember("domain")) {
        Value& domain = json["domain"];
        auto& diag_map_domain = diag_map_[Diag::domain];
        
        diag_map_domain["hello"] = domain["hello"].GetString();
        diag_map_domain["perfectly_legal"] = domain["perfectly_legal"].GetString();
        diag_map_domain["partly_illegal"]  = domain["partly_illegal"].GetString();
        diag_map_domain["totally_illegal"] = domain["totally_illegal"].GetString();
    }

    if(json.HasMember("subdomain")) {
        Value& subdomain = json["subdomain"];
        auto& diag_map_subdomain = diag_map_[Diag::subdomain];

        diag_map_subdomain["success_true"]  = subdomain["success_true"].GetString();
        diag_map_subdomain["success_false"] = subdomain["success_false"].GetString();
        // diag_map_subdomain["fail"]          = subdomain["fail"].GetString();
    }

    if(json.HasMember("service")) {
        Value& service = json["service"];
        auto& diag_map_serivce = diag_map_[Diag::service];

        diag_map_serivce["perfectly_legal"] = service["perfectly_legal"].GetString();
        diag_map_serivce["partly_illegal"]  = service["partly_illegal"].GetString();
        diag_map_serivce["totally_illegal"] = service["totally_illegal"].GetString();
        diag_map_serivce["multi_illegal"]   = service["multi_illegal"].GetString();
        diag_map_serivce["connect_illegal"] = service["connect_illegal"].GetString();
    }
    
    if(json.HasMember("subservice")) {
        Value& subservice = json["subservice"];
        auto& diag_map_subservice = diag_map_[Diag::subservice];

        diag_map_subservice["success_true"]           = subservice["success_true"].GetString();
        diag_map_subservice["success_true_not_found"] = subservice["success_true_not_found"].GetString();
        diag_map_subservice["success_false"]          = subservice["success_false"].GetString();
        diag_map_subservice["fail"]                   = subservice["fail"].GetString();
    }

    if(json.HasMember("servicerollback")) {
        Value& rollback = json["servicerollback"];
        auto& diag_map_rollback = diag_map_[Diag::servicerollback];

        diag_map_rollback["success_true"]  = rollback["success_true"].GetString();
        diag_map_rollback["success_false"] = rollback["success_false"].GetString();
    }

    if(json.HasMember("bandwidth")) {
        Value& bandwidth = json["bandwidth"];
        auto& diag_map_bandwidth = diag_map_[Diag::bandwidth];

        diag_map_bandwidth["illegal"] = bandwidth["illegal"].GetString();
        diag_map_bandwidth["error"]   = bandwidth["error"].GetString();
        diag_map_bandwidth["legal"]   = bandwidth["legal"].GetString();
    }

    if(json.HasMember("bandwidthrollback")) {
        Value& rollback = json["bandwidthrollback"];
        auto& diag_map_rollback = diag_map_[Diag::bandwidthrollback];

        diag_map_rollback["success_true"]  = rollback["success_true"].GetString();
        diag_map_rollback["success_false"] = rollback["success_false"].GetString();
    }
}

std::string DiagMgr::GetDiag(Diag diag, const std::string& condition) {
    if(!HasDiag(diag, condition)) {
        LOG_INFO << "Cannot find diag.";
        return "";
    }
    return diag_map_[diag][condition];

}

bool DiagMgr::HasDiag(Diag diag, const std::string& condition) {
    auto found = diag_map_.find(diag) != diag_map_.end() && diag_map_[diag].find(condition) != diag_map_[diag].end();
    return found;
}

} // !namespace ibn
