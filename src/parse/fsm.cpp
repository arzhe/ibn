#include <algorithm>

#include "fsm.h"
#include "base/util/util.h"
#include "log/logging.h"

namespace ibn {

void FsmState::DomainState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();
    fsm->is_completed_ = false;

    if(fsm->pmgr_->HasDomain(intend)) {
        fsm->ChangeState(&FsmState::ServiceState);
        LOG_TRACE << "[Domain] >> [Service]";
        
        fsm->RestoreDomain(intend);
        std::string diag = fsm->GetDiag(Diag::domain, "perfectly_legal");
        diag = diag.replace(diag.find("*"), 1, intend);
        reply = ColorString<BLUE>("<IBN>") + diag;
    }
    else if(fsm->pmgr_->HasDomainBySimilarity(intend)){
        auto domains = fsm->pmgr_->GetAllDomainsBySimilarity(intend);

        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::domain, "partly_illegal");
        std::string hint;

        for(int i = 0; i < domains.size(); ++i) {
            if(i == domains.size() - 1) {
                hint += "[" + domains[i] + "]";
            }
            else {
                hint += "[" + domains[i] + "]" + " ";
            }
        }
        reply += "\n" + ColorString<RED>("<HINT>") + ColorString<RED>(hint);

        if(domains.size() == 1) {
            fsm->ChangeState(&FsmState::SubDomainState);
            fsm->RestoreDomain(domains[0]);
            LOG_TRACE << "[Domain] >> [SubDomain]";
        }
        else {
            LOG_TRACE << "[Domain] >> [Domain]";
        }
    }
    else {
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::domain, "totally_illegal");
        LOG_TRACE << "[Domain] >> [Domain]";
    }
    fsm->SetReply(reply);
}

void FsmState::SubDomainState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();

    bool l_true = IbnUtil::LogicTrue(intend);
    bool l_false = IbnUtil::LogicFalse(intend);

    if(l_true) {
        fsm->ChangeState(&FsmState::ServiceState);
        LOG_TRACE << "[SubDomain] >> [Service]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subdomain, "success_true");
        reply = reply.replace(reply.find("*"), 1, fsm->GetDomainFromCache());

        fsm->SetReply(reply);
    }
    else if(l_false) {
        fsm->ChangeState(&FsmState::DomainState);
        LOG_TRACE << "[SubDomain] >> [Domain]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subdomain, "success_false");

        fsm->SetReply(reply);
    }
    else {
        fsm->ChangeState(&FsmState::DomainState);
        LOG_TRACE << "[SubDomain] >> [Domain], but handled by DomainState.";
        fsm->Handle(intend);
        // reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subdomain, "fail");
    }
}

void FsmState::ServiceState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();
    std::string domain = fsm->GetDomainFromCache();

    auto intend_slice = IbnUtil::Split(intend, " ");

    auto found_key_to  = std::find(intend_slice.begin(), intend_slice.end(), "to");
    auto found_key_and = std::find(intend_slice.begin(), intend_slice.end(), "and");
    auto found_key     = found_key_to != intend_slice.end() ? found_key_to : 
                        (found_key_and != intend_slice.end() ? found_key_and : intend_slice.end());
    
    bool legal_position = (found_key != intend_slice.begin()) && found_key != (intend_slice.end() - 1);
    bool legal_size = (intend_slice.size() == 3);
    bool legal = legal_position && legal_size;

    if(found_key != intend_slice.end() && legal) {
        std::string node1 = intend_slice[0];
        std::string node2 = intend_slice[2];

        if(fsm->pmgr_->HasLink(domain, node1, node2)) {
            fsm->ChangeState(&FsmState::BandwidthState);
            LOG_TRACE << "[Service] >> [Bandwidth]";
            reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::service, "perfectly_legal");
            reply = reply.replace(reply.find("*"), 1, node1);
            reply = reply.replace(reply.find("#"), 1, node2);

            fsm->RestoreNodePair(node1, node2);
        }
        else if(fsm->pmgr_->HasNodeByName(node1) && fsm->pmgr_->HasNodeByName(node2)) {
            fsm->ChangeState(&FsmState::ServiceRollbackState);
            LOG_TRACE << "[Service] >> [ServiceRollback]";
            reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::service, "connect_illegal");
            reply = reply.replace(reply.find("*"), 1, node1);
            reply = reply.replace(reply.find("#"), 1, node2);
        }
        else if(fsm->pmgr_->HasNodeByNameAndSimilarity(node1) && fsm->pmgr_->HasNodeByNameAndSimilarity(node2)) {
            auto nodes1 = fsm->pmgr_->GetAllNodesByNameAndSimilarity(node1);
            auto nodes2 = fsm->pmgr_->GetAllNodesByNameAndSimilarity(node2);

            std::string hint = ColorString<RED>("<HINT>");

            auto nodes1_size = nodes1.size();
            auto nodes2_size = nodes2.size();

            if(!fsm->pmgr_->HasNodeByName(node1)) {
                hint += node1 + ">>";
                for(const auto& n1 : nodes1) {
                    hint += "[" + ColorString<RED>(n1) + "]" + " ";
                }
            }
            if(!fsm->pmgr_->HasNodeByName(node2)) {
                hint += node2 + ">>";
                for(const auto& n2 : nodes2) {
                    hint += "[" + ColorString<RED>(n2) + "]" + " ";
                }
            }

            if(nodes1_size != 1 || nodes2_size != 1) {
                LOG_TRACE << "[Service] >> [Service]";
            }
            else {
                fsm->ChangeState(&FsmState::SubServiceState);
                LOG_TRACE << "[Service] >> [SubService]";
                fsm->RestoreNodePair(nodes1[0], nodes2[0]);
            }
            reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::service, "partly_illegal") + "\n";
            reply += hint;
        }
        else {
            fsm->ChangeState(&FsmState::ServiceRollbackState);
            LOG_TRACE << "[Service] >> [ServiceRollback]";
            reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::service, "connect_illegal");
            reply = reply.replace(reply.find("*"), 1, node1);
            reply = reply.replace(reply.find("#"), 1, node2);
        }

    }
    else {
        LOG_TRACE << "[Service] >> [Service]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::service, "totally_illegal") + "\n";
        reply += ColorString<RED>("<HINT>") + "A to/and B";
    }

    fsm->SetReply(reply);
}

void FsmState::SubServiceState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();

    bool l_true = IbnUtil::LogicTrue(intend);
    bool l_false = IbnUtil::LogicFalse(intend);
    
    bool found = fsm->pmgr_->HasLink(fsm->GetDomainFromCache(), 
                                     fsm->GetFirstNodeFromCache(), 
                                     fsm->GetSecondNodeFromCache());
    
    if(l_true && found) {
        fsm->ChangeState(&FsmState::BandwidthState);
        LOG_TRACE << "[SubService] >> [Bandwidth]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subservice, "success_true");
        reply = reply.replace(reply.find("*"), 1, fsm->GetFirstNodeFromCache());
        reply = reply.replace(reply.find("#"), 1, fsm->GetSecondNodeFromCache());
    }
    else if(!found) {
        fsm->ChangeState(&FsmState::ServiceRollbackState);
        LOG_TRACE << "[SubService] >> [ServiceRollback]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subservice, "success_true_not_found");
        reply = reply.replace(reply.find("*"), 1, fsm->GetFirstNodeFromCache());
        reply = reply.replace(reply.find("#"), 1, fsm->GetSecondNodeFromCache());
    }
    else if(l_false){
        fsm->ChangeState(&FsmState::ServiceState);
        LOG_TRACE << "[SubService] >> [Service]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subservice, "success_false");
    }
    else {
        fsm->ChangeState(&FsmState::ServiceState);
        LOG_TRACE << "[SubService] >> [Service]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::subservice, "fail");
    }

    fsm->SetReply(reply);
}

void FsmState::ServiceRollbackState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();

    bool l_true = IbnUtil::LogicTrue(intend);
    bool l_false = IbnUtil::LogicFalse(intend);

    if(l_true) {
        fsm->ChangeState(&FsmState::DomainState);
        LOG_TRACE << "[ServiceRollback] >> [Domain]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::servicerollback, "success_true");

        fsm->SetReply(reply);
    }
    else if(l_false) {
        fsm->ChangeState(&FsmState::ServiceState);
        LOG_TRACE << "[ServiceRollback] >> [Service]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::servicerollback, "success_false");

        fsm->SetReply(reply);
    }
    else {
        LOG_TRACE << "[ServiceRollback] >> [Service], handled by DomainState.";
        fsm->ChangeState(&FsmState::ServiceState);
        fsm->Handle(intend);
    }
}

void FsmState::BandwidthState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();

    if(!IbnUtil::IsNum(intend)) {
        fsm->ChangeState(&FsmState::BandwidthRollbackState);
        LOG_TRACE << "[Bandwidth] >> [BandwidthRollback]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::bandwidth, "error");
    }
    else {
        std::string domain = fsm->GetDomainFromCache();
        int bandwidth = std::stoi(intend);
        if(fsm->pmgr_->IsLegalBandwidth(domain, bandwidth)) {
            fsm->ChangeState(&FsmState::DomainState);
            LOG_TRACE << "[Bandwidth] >> [Domain]";
            reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::bandwidth, "legal");
            
            std::string node1 = fsm->GetFirstNodeFromCache();
            std::string node2 = fsm->GetSecondNodeFromCache();
            reply = reply.replace(reply.find("*"), 1, domain);
            reply = reply.replace(reply.find("#"), 1, node1);
            reply = reply.replace(reply.find("$"), 1, node2);
            reply = reply.replace(reply.find("%"), 1, intend);

            fsm->is_completed_ = true;
        }
        else {
            fsm->ChangeState(&FsmState::BandwidthRollbackState);
            LOG_TRACE << "[Bandwidth] >> [BandwidthRollback]";
            reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::bandwidth, "illegal") + "\n";
            
            int bandwidth = fsm->pmgr_->GetBandwidthByDomain(domain);
            reply += ColorString<RED>("<HINT>") + "0 ~ " + std::to_string(bandwidth) + "Gb/s";
            reply = reply.replace(reply.find("*"), 1, domain);
        }
    }

    fsm->SetReply(reply);
}

void FsmState::BandwidthRollbackState(std::shared_ptr<Fsm> fsm) {
    std::string reply;
    std::string intend = fsm->GetIntend();

    bool l_true = IbnUtil::LogicTrue(intend);
    bool l_false = IbnUtil::LogicFalse(intend);

    if(l_true) {
        fsm->ChangeState(&FsmState::DomainState);
        LOG_TRACE << "[BandwidthRollback] >> [Domain]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::bandwidthrollback, "success_true");

        fsm->SetReply(reply);
    }
    else if(l_false) {
        fsm->ChangeState(&FsmState::BandwidthState);
        LOG_TRACE << "[BandwidthRollback] >> [Bandwidth]";
        reply = ColorString<BLUE>("<IBN>") + fsm->GetDiag(Diag::bandwidthrollback, "success_false");

        fsm->SetReply(reply);
    }
    else {
        LOG_TRACE << "[BandwidthRollback] >> [Bandwidth], handled by BandwidthState.";
        fsm->ChangeState(&FsmState::BandwidthState);
        fsm->Handle(intend);
    }

}

std::shared_ptr<Fsm> Fsm::NewFsm() {
    auto fsm = std::make_shared<Fsm>();
    return fsm;
}

std::shared_ptr<Fsm> Fsm::NewFsmFromFile(const std::string& path) {
    auto fsm = Fsm::NewFsm();
    fsm->pmgr_ = PolicyMgr::NewPolicyMgrFromFile(path);
    fsm->Init();
    
    return fsm;
}

void Fsm::Init() {
    state_ = &FsmState::DomainState;
    dmgr_ = DiagMgr::NewDiagMgr();
    cache_ = Cache::NewCache();
    reply_ = ColorString<BLUE>("<IBN>") + GetDiag(Diag::domain, "hello");
}

void Fsm::Handle(std::string intend) {
    intend_ = IbnUtil::Trim(intend);
    state_(shared_from_this());
}

void Fsm::RestoreDomain(const std::string& domain) {
    cache_->SetDomain(domain);
}

std::string Fsm::GetDomainFromCache() {
    return cache_->GetDomain();
}

void Fsm::RestoreNodePair(const std::string& node1, const std::string& node2) {
    cache_->SetNodePair(node1, node2);
}

std::string Fsm::GetFirstNodeFromCache() {
    return cache_->GetFirstNode();
}

std::string Fsm::GetSecondNodeFromCache() {
    return cache_->GetSecondNode();
}

std::string Fsm::GetDiag(Diag diag, const std::string& condition) {
    return dmgr_->GetDiag(diag, condition);
}

bool Fsm::IsCompleted() {
    return is_completed_;
}

int Fsm::count_ = 0;

} // !namespace ibn
