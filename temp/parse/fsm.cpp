#include <algorithm>

#include "parse/fsm.h"
#include "log/logging.h"

namespace ibn {

FSM::FSM() {
    LOG_INFO << "RATIO is : " << InteractSayings::Similarity::RATIO;
    this->state_ = std::make_shared<DomainState>();
    this->request_ = InteractSayings::DomainState::REQUEST;
    this->cache_ = Cache::NewCache();
}

FSM::FSM(std::shared_ptr<PolicyMgr> pmgr) : FSM() {
    this->pmgr_ = pmgr;
}

FSM::FSM(const std::string& path) : FSM() {
    this->pmgr_ = PolicyMgr::NewPolicyMgrFromFile(path);
}

std::shared_ptr<FSM> FSM::NewFSM() {
    return std::make_shared<FSM>();
}

std::shared_ptr<FSM> FSM::NewFSMFromFile(const std::string& path) {
    return std::make_shared<FSM>(path);
}

void FSM::Handle(std::string intend) {
    state_->Handle(shared_from_this(), intend);
}

void FSM::StateChange(std::shared_ptr<State> state) {
    state_ = state;
}

void DomainState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {
    std::string domain = IbnUtil::Trim(intend);
    this->fsm_ = fsm;
    
    std::shared_ptr<FSM> fsm_spt = fsm_.lock();
    if(fsm_.expired()) {
        LOG_SYSERR << 
            "State Machine has been expired! There is something wrong with the state: DomainState.";
    }

    // State Machine is available.
    std::string request;
    if(fsm_spt->pmgr_->HasDomain(domain)) {
        fsm_spt->StateChange(std::make_shared<ServiceState>());
        LOG_INFO << "State changed from Domain to Service";
        request = InteractSayings::ServiceState::REQUEST + 
                  InteractSayings::Symbol::ENDL + 
                  InteractSayings::ServiceState::HINT;
        fsm_spt->cache_->SetDomain(domain);
    }
    else {
        fsm_spt->StateChange(std::make_shared<SubDomainState>());
        LOG_INFO << "State changed from Domain to SubDomain";
        request = InteractSayings::SubDomainState::REQUEST;
    }
    fsm_spt->SetRequest(request);
}

void SubDomainState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {
    std::string domain = IbnUtil::Trim(intend);
    this->fsm_ = fsm;

    std::shared_ptr<FSM> fsm_spt = fsm_.lock();
    if(fsm_.expired()) {
        LOG_SYSERR << "State Machine has been expired! There is something wrong with the state: SubDomainState.";
    }

    //State Machine is available.
    std::string request;
    if(fsm_spt->pmgr_->HasDomain(domain)) {
        fsm_spt->StateChange(std::make_shared<ServiceState>());
        LOG_INFO << "State changed from SubDomain to Service";
        request = InteractSayings::ServiceState::REQUEST + 
                  InteractSayings::Symbol::ENDL + 
                  InteractSayings::ServiceState::HINT;
        fsm_spt->cache_->SetDomain(domain);
    }
    else if(fsm_spt->pmgr_->HasDomainBySimilarity(domain, InteractSayings::Similarity::RATIO)) {
        std::vector<std::string> relative_domains;
        relative_domains = fsm_spt->pmgr_->GetAllDomainsBySimilarity(domain, InteractSayings::Similarity::RATIO);

        request = InteractSayings::SubDomainState::REQUEST + 
                  InteractSayings::Symbol::ENDL;
        request += InteractSayings::SubDomainState::HINT;
        for(int i = 0; i < relative_domains.size(); ++i) {
            std::string dom = "[" + relative_domains[i] + "]";
            request += ColorString<RED>(dom);
            if(i != relative_domains.size() - 1) {
                request += ", ";
            }
        }
        LOG_INFO << "State stayed at SubDomain";
    }

    fsm_spt->SetRequest(request);
}

void ServiceState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {
    this->fsm_ = fsm;

    std::shared_ptr<FSM> fsm_spt = fsm_.lock();
    if(fsm_.expired()) {
        LOG_SYSERR << "State Machine has been expired! There is something wrong with the state: ServiceState.";
    }

    //State Machine is available.
    std::string request;
    std::vector<std::string> intend_slice = IbnUtil::Split(intend, " ");

    auto intend_slice_connect_it  = std::find(intend_slice.begin(), intend_slice.end(), "connect");
    if(intend_slice_connect_it != intend_slice.end()) {
        auto intend_slice_to_it = std::find(intend_slice_connect_it, intend_slice.end(), "to");
        auto intend_slice_and_it = std::find(intend_slice_connect_it, intend_slice.end(), "and");

        if(intend_slice_to_it != intend_slice.end() || intend_slice_and_it != intend_slice.end()) {
            fsm_spt->StateChange(std::make_shared<ConnectState>());
            LOG_INFO << "State changed from Service to Connect";
            request = "";
        }
        else {
            LOG_INFO << "State stayed at Service";
            request = InteractSayings::SubDomainState::REQUEST + InteractSayings::Symbol::ENDL + InteractSayings::SubServiceState::HINT2;
        }
    }
    else {
        LOG_INFO << "State stayed at Service";
        request = InteractSayings::SubServiceState::REQUEST + InteractSayings::Symbol::ENDL + InteractSayings::SubServiceState::HINT1;
    }
    
    fsm_spt->SetRequest(request);
}

void SubServiceState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {
    std::string domain = IbnUtil::Trim(intend);
    this->fsm_ = fsm;

    std::shared_ptr<FSM> fsm_spt = fsm_.lock();
    if(fsm_.expired()) {
        LOG_SYSERR << "State Machine has been expired! There is something wrong with the state: SubServiceState.";
    }

    std::string request;
    request = "Connection between Wuhan and Guangdong has been built.";
    fsm_spt->SetRequest(request);
}

void ConnectState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {
    this->fsm_ = fsm;

    std::shared_ptr<FSM> fsm_spt = fsm_.lock();
    if(fsm_.expired()) {
        LOG_SYSERR << "State Machine has been expired! There is something wrong with the state: ConnectState.";
    }

    std::string request;
    std::vector<std::string> intend_slice = IbnUtil::Split(intend, " ");
    auto intend_slice_to_it = std::find(intend_slice.begin(), intend_slice.end(), "to");
    auto intend_slice_and_it = std::find(intend_slice.begin(), intend_slice.end(), "and");

    std::string str1, str2;
    if(intend_slice_to_it != intend_slice.begin() && intend_slice_to_it != intend_slice.end()) {
        auto it1 = intend_slice_to_it - 1;
        auto it2 = intend_slice_to_it + 1;
        str1 = *it1;
        str2 = *it2;
    }
    else if(intend_slice_and_it != intend_slice.begin() && intend_slice_and_it != intend_slice.end()) {
        auto it1 = intend_slice_and_it - 1;
        auto it2 = intend_slice_and_it + 1;
        str1 = *it1;
        str2 = *it2;
    }
    
    std::string domain = fsm_spt->cache_->GetDomain();
    LOG_INFO << "Domain stored by Cache is: " << domain;
    LOG_INFO << "Node1  get:                " << str1;
    LOG_INFO << "Node2  get:                " << str2;

    if(fsm_spt->pmgr_->HasLink(domain, str1, str2)) {
        fsm_spt->StateChange(std::make_shared<BandwidthState>());
        LOG_INFO << "State changed from Connect to Bandwidth";
        request = InteractSayings::BandwidthState::REQUEST;
    }
    else if(fsm_spt->pmgr_->HasNodeByNameAndSimilarity(str1, InteractSayings::Similarity::RATIO) && 
            fsm_spt->pmgr_->HasNodeByNameAndSimilarity(str2, InteractSayings::Similarity::RATIO)){
        fsm_spt->StateChange(std::make_shared<SubConnectState>());
        LOG_INFO << "State changed from Connect to SubConnect";
        request = "";
    }
    else {
        fsm_spt->StateChange(std::make_shared<ServiceState>());
        LOG_INFO << "State changed from Connect to Service";
        request = InteractSayings::ConnectState::REQUEST + InteractSayings::Symbol::ENDL + InteractSayings::ConnectState::CHECK;
    }
    fsm_spt->cache_->SetNodePair(str1, str2);

    fsm_spt->SetRequest(request);
}

void SubConnectState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {
    this->fsm_ = fsm;

    std::shared_ptr<FSM> fsm_spt = fsm_.lock();
    if(fsm_.expired()) {
        LOG_SYSERR << "State Machine has been expired! There is something wrong with the state: SubConnectState.";
    }
    
    std::string request;
    
    std::string node1 = fsm_spt->cache_->GetNodePair().first;
    std::string node2 = fsm_spt->cache_->GetNodePair().second;
    LOG_INFO << "Node1 stored by Cache is: " << node1;
    LOG_INFO << "Node2 stored by Cache is: " << node2;

    std::string domain = fsm_spt->cache_->GetDomain();
    if(fsm_spt->pmgr_->HasLink(domain, node1, node2)) {
        fsm_spt->StateChange(std::make_shared<BandwidthState>());
        LOG_INFO << "State changed from Connect to Bandwidth";
        request = InteractSayings::BandwidthState::REQUEST;
    }

    std::vector<std::string> nodes1;
    std::vector<std::string> nodes2;

    nodes1 = fsm_spt->pmgr_->GetAllNodesByNameAndSimilarity(node1, InteractSayings::Similarity::RATIO);
    nodes2 = fsm_spt->pmgr_->GetAllNodesByNameAndSimilarity(node2, InteractSayings::Similarity::RATIO);

    request = InteractSayings::SubConnectState::HINT;
    for(int i = 0; i < nodes1.size(); ++i) {
        std::string n = "[" + nodes1[i] + "]";
        n = ColorString<RED>(n);
        if(i != nodes1.size() - 1) {
            n += ", ";
        }
        request += n;
    }

    request += " and ";
    for(int i = 0; i < nodes2.size(); ++i) {
        std::string n = "[" + nodes2[i] + "]";
        n = ColorString<RED>(n);
        if(i != nodes1.size() - 1) {
            n += ", ";
        }
        request += n;
    }

    fsm_spt->StateChange(std::make_shared<ConnectState>());
    fsm_spt->SetRequest(request);
}

void BandwidthState::Handle(std::shared_ptr<FSM> fsm, std::string intend) {}

const int InteractSayings::Similarity::RATIO       = 50;

const std::string InteractSayings::Symbol::ENDL    = "\n";
const std::string InteractSayings::Symbol::COLON   = ":";

const std::string InteractSayings::Character::IBN  = ColorString<GREEN>("<IBN>");
const std::string InteractSayings::Character::HINT = ColorString<RED>("<HINT>");

const std::string InteractSayings::KeyWord::CONNECT    = ColorString<RED>("[connect]");
const std::string InteractSayings::KeyWord::DISCONNECT = ColorString<RED>("[disconnect]");
const std::string InteractSayings::KeyWord::TO_AND     = ColorString<RED>("[to/and]");
const std::string InteractSayings::KeyWord::TO         = ColorString<RED>("[to]");
const std::string InteractSayings::KeyWord::FROM       = ColorString<RED>("[from]");

const std::string InteractSayings::DomainState::REQUEST = InteractSayings::Character::IBN + "Tell me your project name?";

const std::string InteractSayings::SubDomainState::REQUEST = InteractSayings::Character::IBN + "Sorry, I can't recognize your intend.";
const std::string InteractSayings::SubDomainState::HINT    = InteractSayings::Character::HINT + "Do you mean to say the following domains:" +
                                                             InteractSayings::Symbol::ENDL + InteractSayings::Character::HINT;

const std::string InteractSayings::ServiceState::REQUEST = InteractSayings::Character::IBN + "What kind of net-service do you want to do?";
const std::string InteractSayings::ServiceState::HINT    = InteractSayings::Character::HINT + "You can use the key word to access to the net-service:" +
                                                           InteractSayings::Symbol::ENDL + InteractSayings::Character::HINT +
                                                           InteractSayings::KeyWord::CONNECT + " or " + InteractSayings::KeyWord::DISCONNECT + 
                                                           InteractSayings::Symbol::ENDL + InteractSayings::Character::HINT +
                                                           "And you should use the key word to imply your connection/disconnection intend:" +
                                                           InteractSayings::Symbol::ENDL + InteractSayings::Character::HINT + InteractSayings::KeyWord::TO_AND;

const std::string InteractSayings::SubServiceState::REQUEST = InteractSayings::Character::IBN + "Sorry, I can't recognize your intend.";
const std::string InteractSayings::SubServiceState::HINT1   = InteractSayings::Character::HINT + "You should use the key word to access to the net-service:" +
                                                              InteractSayings::Symbol::ENDL + InteractSayings::Character::HINT +
                                                              InteractSayings::KeyWord::CONNECT + " or " + InteractSayings::KeyWord::DISCONNECT;
const std::string InteractSayings::SubServiceState::HINT2   = InteractSayings::Character::HINT + "You should use the key word to imply your connection intend:" +
                                                              InteractSayings::Symbol::ENDL + InteractSayings::KeyWord::TO_AND;

const std::string InteractSayings::ConnectState::REQUEST = InteractSayings::Character::IBN + "Sorry, I can't recognize your connection intend.";
const std::string InteractSayings::ConnectState::CHECK   = InteractSayings::Character::IBN + "Please check your connection name.";


const std::string InteractSayings::SubConnectState::REQUEST = InteractSayings::Character::IBN + "Sorry, I can't find exact connection you asked for.";
const std::string InteractSayings::SubConnectState::HINT    = InteractSayings::Character::HINT + "Do you mean to say following nodes to connect:" +
                                                              InteractSayings::Symbol::ENDL + InteractSayings::Character::HINT;

const std::string InteractSayings::BandwidthState::REQUEST = InteractSayings::Character::IBN + "How much bandwidth do you need?";

} // !namespace ibn
